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
        alignItems(`Center),
        justifyContent(`Center),
        flexDirection(`Row),
        flexGrow(1),
        backgroundColor(currentTimer |> Theme.colorFromTimer(`Bg)),
      ]>
      <Timer />
    </View>
  </AppProvider>;
};
