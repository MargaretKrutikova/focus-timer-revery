open Revery;
open Revery.UI;
open FocusTimer_Store;
open FocusTimer_Models;

type uiTimer = {
  timer: TimerModel.t,
  timeLeft: Time.t,
};

let fullTime = timer => Settings.getDuration(AppStore.settings, timer);
let getTimeLeft = (timer, elapsed) =>
  fullTime(timer) -. elapsed |> Time.ofFloatSeconds;

let toUiTimer = (state: TimerStore.state): uiTimer => {
  switch (state) {
  | TimerRunningState({timer, elapsed, _}) => {
      timer,
      timeLeft: getTimeLeft(timer, elapsed),
    }
  | TimerScheduledState({timer, _} as ts) =>
    let elapsed = TimerStore.TimerScheduledData.toElapsed(ts);
    {timer, timeLeft: getTimeLeft(timer, elapsed)};
  | TimerElapsedState({timer, _}) => {
      timer,
      timeLeft: fullTime(timer) |> Time.ofFloatSeconds,
    }
  | TimerPausedState({timer, elapsed, _}) => {
      timer,
      timeLeft: getTimeLeft(timer, elapsed),
    }
  | Idle(timer) => {timer, timeLeft: fullTime(timer) |> Time.ofFloatSeconds}
  };
};

let selector = state => state;
let%component make = () => {
  let%hook timerState = AppStore.useSelector(selector);
  let timerDispatch = AppStore.dispatch;

  let uiTimer = toUiTimer(timerState);
  <View>
    <AppButton
      label="Start"
      onClick={_ => timerDispatch(TimerStore.StartTimers)}
    />
    <Clock.Interval time={uiTimer.timeLeft} />
    <AppButton
      label="Pause"
      onClick={_ => timerDispatch(TimerStore.TimerPaused)}
    />
    <AppButton
      label="Resume"
      onClick={_ => timerDispatch(TimerStore.TimerResumed)}
    />
  </View>;
};
