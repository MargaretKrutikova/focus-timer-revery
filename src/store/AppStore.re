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

type appState = {timer: TimerState.state};
let initialAppState = {
  timer: TimerState.initialState(settings.defaultTimer),
};

let timerUpdater = TimerStore.StateMachine.run(~settings, ~clock);
let appUpdater = (state: appState, action) => {
  let (timer, effect) = timerUpdater(state.timer, action);
  let state = {timer: timer};
  (state, effect);
};

include Core.Make({
  type action = AppActions.t;
  type state = appState;

  let updater = appUpdater;
  let initialState = (initialAppState, Effect.none);
});
