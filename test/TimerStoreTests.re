open TestFramework;
open FocusTimer_Store;
open FocusTimer_Models;
open TimerStore;
open TimerState;

let clock: clock = () => 0.0;

describe("Timer store", t => {
  t.test("starts a default time from idle", e => {
    let defaultTimer = TimerModel.Focus;
    let settings: Settings.t = {
      focusDurationSec: 10.00,
      restDurationSec: 50.00,
      defaultTimer,
      focusTimerElapseSoundPath: "",
      restTimerElapseSoundPath: "",
    };

    let initialState = initialState(defaultTimer);
    let reducer = StateMachine.run(~settings, ~clock);
    let (state, _) = reducer(initialState, StartTimers);

    let expected =
      TimerScheduledState(TimerScheduledData.make(NewTimer, defaultTimer));
    let isExpected = isStateEqual(expected, state);

    e.expect.bool(isExpected).toBeTrue();
  })
});
