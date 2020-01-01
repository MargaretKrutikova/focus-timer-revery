open Revery
open FocusTimer_Core
open FocusTimer_Utils

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
  TimerScheduled (type_ |> timerWithData timer), Effects.startTimer ()

let pauseTimer (clock: clock) timer (tr: timerRunningData) : (state * action Effect.t)  =
  TimerPausedState ({ started = tr.started; elapsed=tr.elapsed; paused = clock() } |> timerWithData timer),
  Effects.pauseTimer tr.dispose

let nextTick settings timer (tr: timerRunningData) tick : (state * action Effect.t) =
  let elapsed = tr.elapsed +. tick in
  if elapsed > Settings.getDuration settings timer
  then 
    TimerElapsedState {timer;data=()},
    Effect.batch [
      Effects.playTimerElapseSound settings timer; 
      Effects.stopTimer tr.dispose; 
      Effects.startTimer ()
    ]
  else 
    TimerRunningState ({ tr with elapsed=elapsed} |> timerWithData timer), 
    Effect.none

let toElapsed =
  function
  | NewTimer -> 0.0
  | ResumedTimer elapsed ->  elapsed 

module StateMachine = struct
    let run ~settings ~clock:(clock:clock) state action : (state * action Effect.t) = 
      (* let x = match state with
      | TimerElapsedState _ -> Printf.printf "%s" "TIMER: Elapsed\n"
      | TimerRunningState {timer;data} -> Printf.printf "%s" ("TIMER: Running at " ^ (data.elapsed |> string_of_float) ^ " \n")
      | TimerScheduled _ -> Printf.printf "TIMER: Sceduled\n"
      | TimerPausedState _ -> Printf.printf "TIMER: Paused\n"
      | Idle _ ->  Printf.printf "TIMER: Idle\n"
      in *)

      match action with
      | StartTimers -> 
      (match state with
        | Idle defaultTimer -> scheduleTimer NewTimer defaultTimer
        | _ -> (state, Effect.none))
      | Tick tick -> 
        (match state with 
        | TimerRunningState {timer;data} -> 
          nextTick settings timer data tick
        | _ -> (state, Effect.none))
      | TimerStarted dispose -> 
        (match state with
        | TimerScheduled {timer;data} ->
          let trd = {started=clock (); elapsed=toElapsed data; dispose} |> timerWithData timer in
          (TimerRunningState trd, Effect.none)
        | _ -> (state, Effect.none))
      | TimerElapsed ->
         (match state with
          | TimerElapsedState {timer;_} -> TimerModel.next timer |> scheduleTimer NewTimer
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
