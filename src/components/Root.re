open Revery;
open Revery.UI;
open FocusTimer_Store;

let selector = (state: AppStore.appState) =>
  TimerState.currentTimer(state.timer);

let%component make = () => {
  let%hook currentTimer = AppStore.useSelector(selector);

  <AppProvider>
    <View
      style=Style.[
        position(`Absolute),
        bottom(0),
        top(0),
        left(0),
        right(0),
        justifyContent(`Center),
        backgroundColor(currentTimer |> Theme.colorFromTimer(`Bg)),
      ]>
      <Timer />
    </View>
  </AppProvider>;
};
