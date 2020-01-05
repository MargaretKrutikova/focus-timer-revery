open Revery;
open Revery.UI;
open FocusTimer_Store;
open FocusTimer_Utils;

module EffectProvider = {
  let%component make = (~children: Brisk_reconciler.element(Revery_UI.viewNode), ()) => {
    let%hook () = AppStore.useRunEffects();
    children;
  };
};

module AudioProvider = {
  let%component make = (~children, ()) => {
    let%hook () =
      Hooks.effect(
        OnMount,
        () => {
          Audio.initSound();
          Some(Audio.endSound);
        },
      );
    children;
  };
};

let make = (~children, ()) => {
  <EffectProvider>
    <AudioProvider> ...children </AudioProvider>
  </EffectProvider>;
};
