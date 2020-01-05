open Revery;
open Revery.UI;
open FocusTimer_Components;

let init = app => {
  let _ = Revery.Log.listen((_, msg) => print_endline("LOG: " ++ msg));
  let win = App.createWindow(app, "Focus Timer");
  Window.setRawSize(win, 400, 300);

  let _ = UI.start(win, <Root />);
  ();
};

let start = () => App.start(init);
