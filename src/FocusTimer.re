open Revery;
open Revery.UI;
open FocusTimer_Components;
open FocusTimer_Store;

module EffectProvider = {
  let%component make = (~children, ()) => {
    let%hook () = AppStore.useRunEffects();
    children;
  };
};

let init = app => {
  let _ = Revery.Log.listen((_, msg) => print_endline("LOG: " ++ msg));

  let win = App.createWindow(app, "Welcome to Revery!");

  let containerStyle =
    Style.[
      position(`Absolute),
      justifyContent(`Center),
      alignItems(`Center),
      bottom(0),
      top(0),
      left(0),
      right(0),
    ];

  let element =
    <View style=containerStyle>
       <EffectProvider><Timer /></EffectProvider>
    </View>;

  let _ = UI.start(win, element);
  ();
};
let start = () => App.start(init);
