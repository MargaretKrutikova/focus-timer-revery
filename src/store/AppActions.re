open FocusTimer_Models;

type t =
  /* timers */
  | StartTimers
  | Tick((TimerModel.t, float))
  | TimerSkipped
  | TimerStarted(TimerState.dispose)
  | TimerPaused
  | TimerResumed;
