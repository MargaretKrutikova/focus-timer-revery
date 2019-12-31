open FocusTimer_Core;

let clock = Unix.gettimeofday;

// temporary
let settings: Settings.t = {
  focusDurationSec: 10.0,
  restDurationSec: 10.0,
  defaultTimer: TimerModel.Focus,
  focusTimerElapseSoundPath: "drum.wav",
  restTimerElapseSoundPath: "drum.wav",
};

let timerUpdater = TimerLogic.StateMachine.run(~settings, ~clock);

include Core.Make({
  type action = TimerLogic.action;
  type state = TimerLogic.state;

  let updater = timerUpdater;

  let initialState = (
    TimerLogic.initialState(settings.defaultTimer),
    Effect.none,
  );
});
