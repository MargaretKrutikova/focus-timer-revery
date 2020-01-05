open Revery;
open Revery.UI;
open FocusTimer_Components;

let init = app => {
  let _ = Revery.Log.listen((_, msg) => print_endline("LOG: " ++ msg));
  let win = App.createWindow(app, "Focus Timer");

  let containerStyle =
    Style.[position(`Absolute), bottom(0), top(0), left(0), right(0)];
  let element = <View style=containerStyle> <Root /> </View>;

  let _ = UI.start(win, element);
  ();
};
let start = () => App.start(init);
