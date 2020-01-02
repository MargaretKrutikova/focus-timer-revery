open FocusTimer_Core;
open FocusTimer_Models;

let clock = Unix.gettimeofday;

/* temporary */
let settings: Settings.t = {
  focusDurationSec: 10.0,
  restDurationSec: 10.0,
  defaultTimer: TimerModel.Focus,
  focusTimerElapseSoundPath: "drum.wav",
  restTimerElapseSoundPath: "drum.wav",
};

let timerUpdater = TimerStore.StateMachine.run(~settings, ~clock);

include Core.Make({
  type action = TimerStore.action;
  type state = TimerStore.state;

  let updater = timerUpdater;

  let initialState = (
    TimerStore.initialState(settings.defaultTimer),
    Effect.none,
  );
});
