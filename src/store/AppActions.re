type t =
  /* timers */
  | StartTimers
  | Tick(float)
  | TimerStarted(TimerState.dispose)
  | TimerPaused
  | TimerResumed
  | TimerElapsed
  | TimerStopped;
