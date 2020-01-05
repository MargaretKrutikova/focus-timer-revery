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

let toUiTimer = (state: TimerState.state): uiTimer => {
  switch (state) {
  | TimerRunningState({timer, elapsed, _})
  | TimerPausedState({timer, elapsed, _}) =>
    let timeLeft = getTimeLeft(timer, elapsed);
    {timer, timeLeft};
  | TimerScheduledState({timer, _} as ts) =>
    let elapsed = TimerState.TimerScheduledData.toElapsed(ts);
    {timer, timeLeft: getTimeLeft(timer, elapsed)};
  | Idle(timer) => {timer, timeLeft: fullTime(timer) |> Time.ofFloatSeconds}
  };
};

let selector = (state: AppStore.appState) => state.timer;
let%component make = () => {
  let%hook timerState = AppStore.useSelector(selector);
  let timerDispatch = AppStore.dispatch;

  let uiTimer = toUiTimer(timerState);
  <View style=Style.[justifyContent(`Center), alignItems(`Center)]>
    <Clock
      time={uiTimer.timeLeft}
      currentTimer={TimerState.currentTimer(timerState)}
    />
    {switch (timerState) {
     | TimerPausedState(_) =>
       <AppButton
         label="Resume"
         onClick={_ => timerDispatch(AppActions.TimerResumed)}
       />
     | TimerRunningState(_)
     | TimerScheduledState(_) =>
       <AppButton
         label="Pause"
         onClick={_ => timerDispatch(AppActions.TimerPaused)}
       />
     | Idle(_) =>
       <AppButton
         label="Start"
         onClick={_ => timerDispatch(AppActions.StartTimers)}
       />
     }}
  </View>;
};
