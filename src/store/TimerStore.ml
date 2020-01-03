open Revery
open FocusTimer_Core
open FocusTimer_Utils
open FocusTimer_Models

type dispose = unit -> unit
type clock = unit -> float

module TimerRunningData = struct
  type t = { dispose: dispose; started: float; elapsed: float; timer: TimerModel.t }

  let make ~started ~elapsed ~dispose timer = {started;elapsed;dispose;timer}
  let setElapsed elapsed data = { data with elapsed=elapsed}
end

module TimerPausedData = struct
  type t = { started: float; elapsed: float; paused: float; timer: TimerModel.t }

  let make ~started ~elapsed ~paused timer = {started;elapsed;paused;timer}
end

module TimerScheduledData = struct
  type type_ = NewTimer | ResumedTimer of float
  type t = { timer: TimerModel.t; type_: type_ }

  let make type_ timer = {timer; type_}
  let toElapsed ts =
    match ts.type_ with
    | NewTimer -> 0.0
    | ResumedTimer elapsed ->  elapsed
end

module TimerElapsedData = struct
  type t = {timer: TimerModel.t}
  let make timer = {timer}
end

type state =
    | Idle of TimerModel.t
    | TimerRunningState of TimerRunningData.t
    | TimerScheduledState of TimerScheduledData.t
    | TimerElapsedState of TimerElapsedData.t
    | TimerPausedState of TimerPausedData.t

let isStateEqual expect actual =
  compare expect actual == 0

let initialState timer = Idle timer

type action =
    | StartTimers
    | Tick of float
    | TimerStarted of dispose
    | TimerPaused
    | TimerResumed
    | TimerElapsed
    | TimerStopped

module Effects = struct
  let stopTimer (dispose: dispose) =
    Effect.create (fun dispatch ->
      dispatch TimerElapsed;
      dispose())

  let pauseTimer (dispose: dispose) =
    Effect.create (fun _ -> dispose())

  let startTimer () =
    Effect.create (fun dispatch ->
      let dispose = Tick.interval
          (fun t -> Tick(t |> Time.toFloatSeconds) |> dispatch)
          (Time.ofFloatSeconds 0.2)
      in
      TimerStarted dispose |> dispatch
    )

  let playTimerElapseSound settings timer =
    Effect.create (fun _ ->
      timer
      |> Settings.getTimerElapseSound settings
      (* TODO: Implement adjusting sound in the app *)
      |> Audio.playSound 0.5
      |> ignore
    )
end

let scheduleTimer type_ timer : (state * action Effect.t) =
  TimerScheduledState (TimerScheduledData.make type_ timer), Effects.startTimer ()

let pauseTimer (clock: clock) (tr: TimerRunningData.t) : (state * action Effect.t) =
  TimerPausedState (TimerPausedData.make ~started:tr.started ~elapsed:tr.elapsed ~paused: (clock ()) tr.timer),
  Effects.pauseTimer tr.dispose

let startTimer clock dispose (ts: TimerScheduledData.t) =
  let elapsed = TimerScheduledData.toElapsed ts in
  let trd = TimerRunningData.make ~started:(clock ()) ~elapsed ~dispose ts.timer in
  (TimerRunningState trd, Effect.none)

let nextTick settings (tr: TimerRunningData.t) tick : (state * action Effect.t) =
  let elapsed = tr.elapsed +. tick in
  if elapsed > Settings.getDuration settings tr.timer
  then
    TimerElapsedState (TimerElapsedData.make tr.timer),
    Effect.batch [
      Effects.playTimerElapseSound settings tr.timer;
      Effects.stopTimer tr.dispose;
      Effects.startTimer ()
    ]
  else
    TimerRunningState (TimerRunningData.setElapsed elapsed tr),
    Effect.none

module StateMachine = struct
    let run ~settings ~clock:(clock:clock) state action : (state * action Effect.t) =
      match action with
      | StartTimers ->
      (match state with
        | Idle defaultTimer -> scheduleTimer NewTimer defaultTimer
        | _ -> (state, Effect.none))
      | Tick tick ->
        (match state with
        | TimerRunningState tr -> nextTick settings tr tick
        | _ -> (state, Effect.none))
      | TimerStarted dispose ->
        (match state with
        | TimerScheduledState ts -> startTimer clock dispose ts
        | _ -> (state, Effect.none))
      | TimerElapsed ->
         (match state with
          | TimerElapsedState te -> TimerModel.next te.timer |> scheduleTimer NewTimer
          | _ -> (state, Effect.none))
      | TimerResumed ->
        (match state with
        | TimerPausedState tp -> scheduleTimer (ResumedTimer tp.elapsed) tp.timer
        | _ -> (state, Effect.none))
      | TimerPaused ->
        (match state with
        | TimerRunningState tr -> pauseTimer clock tr
        | _ -> (state, Effect.none))
      | TimerStopped -> Idle settings.defaultTimer, Effect.none
  end
