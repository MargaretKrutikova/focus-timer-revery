open FocusTimer_Models;

type t =
  /* timers */
  | StartTimers
  | Tick((TimerModel.t, float))
  | TimerStarted(TimerState.dispose)
  | TimerPaused
  | TimerResumed
  | TimerStopped;
