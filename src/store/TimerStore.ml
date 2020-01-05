open Revery
open FocusTimer_Core
open FocusTimer_Utils
open FocusTimer_Models
open TimerState

module Effects = struct
  let stopTimer (dispose: dispose) =
    Effect.create (fun dispatch -> dispose())

  let pauseTimer (dispose: dispose) =
    Effect.create (fun _ -> dispose())

  let startTimer timer =
    Effect.create (fun dispatch ->
      let dispose = Tick.interval
          (fun t -> AppActions.Tick(timer, t |> Time.toFloatSeconds) |> dispatch)
          Time.zero
      in
      AppActions.TimerStarted dispose |> dispatch
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

let scheduleTimer type_ timer : (state * AppActions.t Effect.t) =
  TimerScheduledState (TimerScheduledData.make type_ timer), Effects.startTimer timer

let pauseTimer (clock: clock) (tr: TimerRunningData.t) : (state * AppActions.t Effect.t) =
  TimerPausedState (TimerPausedData.make ~started:tr.started ~elapsed:tr.elapsed ~paused: (clock ()) tr.timer),
  Effects.pauseTimer tr.dispose

let startTimer clock dispose (ts: TimerScheduledData.t) =
  let elapsed = TimerScheduledData.toElapsed ts in
  let trd = TimerRunningData.make ~started:(clock ()) ~elapsed dispose ts.timer in
  (TimerRunningState trd, Effect.none)

let nextTick settings (tr: TimerRunningData.t) tick : (state * AppActions.t Effect.t) =
  let elapsed = tr.elapsed +. tick in
  if elapsed > Settings.getDuration settings tr.timer
  then
    let nextTimer = TimerModel.next tr.timer in
    let (state, effect) = scheduleTimer NewTimer nextTimer in
      state,
      Effect.batch [
        effect;
        Effects.playTimerElapseSound settings tr.timer;
        Effects.stopTimer tr.dispose;
      ]
  else
    TimerRunningState (TimerRunningData.setElapsed elapsed tr),
    Effect.none

module StateMachine = struct
    let run ~settings ~clock:(clock:clock) state action : (state * AppActions.t Effect.t) =
      match action with
      | AppActions.StartTimers ->
      (match state with
        | Idle defaultTimer -> scheduleTimer NewTimer defaultTimer
        | _ -> (state, Effect.none))
      | Tick (timer, tick) ->
        (match state with
        | TimerRunningState tr when tr.timer = timer -> nextTick settings tr tick
        | _ -> (state, Effect.none))
      | TimerStarted dispose ->
        (match state with
        | TimerScheduledState ts -> startTimer clock dispose ts   
        | _ -> (state, Effect.none))
      | TimerResumed ->
        (match state with
        | TimerPausedState tp -> scheduleTimer (ResumedTimer tp.elapsed) tp.timer
        | _ -> (state, Effect.none))
      | TimerPaused ->
        (match state with
        | TimerRunningState tr -> pauseTimer clock tr
        | _ -> (state, Effect.none))
      | TimerSkipped -> Idle settings.defaultTimer, Effect.none
  end
