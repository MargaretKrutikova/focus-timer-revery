open Revery

type dispose = unit -> unit
type clock = unit -> float

type 'data timerData = {
  timer: TimerModel.t;
  data: 'data;
}

let timerWithData timer data =
  { timer; data }

type timerRunningData =
    { dispose: dispose;
      started: float;
      elapsed: float; }

type timerPausedData =
    { started: float;
      elapsed: float;
      paused: float; }

type timerScheduledData = NewTimer | ResumedTimer of float

type timerElapsedData = ()

type state =
    | Idle of TimerModel.t
    | TimerRunningState of timerRunningData timerData
    | TimerScheduled of timerScheduledData timerData
    | TimerElapsedState of timerElapsedData timerData
    | TimerPausedState of timerPausedData timerData

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
          Time.zero
      in
      TimerStarted dispose |> dispatch
    )
end

let scheduleTimer type_ timer : (state * action Effect.t) =
  TimerScheduled (type_ |> timerWithData timer), Effects.startTimer ()

let pauseTimer (clock: clock) timer (tr: timerRunningData) : (state * action Effect.t)  =
  TimerPausedState ({ started = tr.started; elapsed=tr.elapsed; paused = clock() } |> timerWithData timer),
  Effects.pauseTimer tr.dispose

let nextTick duration clock timer (tr: timerRunningData) tick : (state * action Effect.t) =
  let elapsed = tr.elapsed +. tick in
  if elapsed > duration
  then 
    TimerElapsedState {timer;data=()},
    Effect.batch [Effects.stopTimer tr.dispose; Effects.startTimer ()]
  else 
    TimerRunningState ({ tr with elapsed=elapsed} |> timerWithData timer), 
    Effect.none

let toElapsed =
  function
  | NewTimer -> 0.0
  | ResumedTimer elapsed ->  elapsed 

module StateMachine = struct
    let run ~settings ~clock:(clock:clock) state action : (state * action Effect.t) = 
      match action with
      | StartTimers -> 
      (match state with
        | Idle defaultTimer -> scheduleTimer NewTimer defaultTimer
        | _ -> (state, Effect.none))
      | Tick tick -> 
        (match state with 
        | TimerRunningState {timer;data} -> 
          nextTick (Settings.getDuration settings timer) clock timer data tick
        | _ -> (state, Effect.none))
      | TimerStarted dispose -> 
        (match state with
        | TimerScheduled {timer;data} ->
          let trd = {started=clock (); elapsed=toElapsed data; dispose} |> timerWithData timer in
          (TimerRunningState trd, Effect.none)
        | _ -> (state, Effect.none))
      | TimerElapsed ->
         (match state with
          | TimerElapsedState {timer;data} -> TimerModel.next timer |> scheduleTimer NewTimer
          | _ -> (state, Effect.none))
      | TimerResumed ->
        (match state with 
        | TimerPausedState {timer;data} -> scheduleTimer (ResumedTimer data.elapsed) timer
        | _ -> (state, Effect.none))
      | TimerPaused -> 
        (match state with 
        | TimerRunningState {timer;data} -> pauseTimer clock timer data
        | _ -> (state, Effect.none))
      | TimerStopped -> Idle settings.defaultTimer, Effect.none
  end
