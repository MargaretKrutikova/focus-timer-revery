open CoreTypes;
open Revery.UI;
open Revery;

type updater('action, 'state) =
  ('state, 'action) => ('state, Effect.t('action));

module type Config = {
  type state;
  type action;
  let updater: updater(action, state);
  let initialState: (state, Effect.t(action));
};

module Make = (Config: Config) => {
  let (preloadedState, initEffect) = Config.initialState;
  let store = Store.create(~reducer=Config.updater, ~preloadedState, ());

  let subscribe = Store.subscribe(store);
  let dispatch = Store.dispatch(store);

  Effect.run(initEffect, dispatch);

  let useSelector = selector => {
    let%hook (selectorRef, setSelectorRef) =
      Hooks.ref(() => selector(Store.getState(store)));

    let%hook () =
      Hooks.effect(
        If((prev, current) => prev !== current, selector),
        () => {
          setSelectorRef(() => selector(Store.getState(store)));
          None;
        },
      );

    let%hook selectedState =
      Subscription.useSubscription(subscribe, selectorRef);

    selectedState;
  };

  let getEffects = () => Store.getEffects(store);
  let useRunEffects = () => {
    let%hook effects = Subscription.useSubscription(subscribe, getEffects);

    let%hook () =
      Hooks.effect(
        If((prev, current) => prev !== current, effects),
        () => {
          Store.runEffects(store);
          None;
        },
      );
    ();
  };
};
