open CoreTypes;

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
  let store =
    Store.create(~reducer=(state, _) => state, ~preloadedState, ());

  let subscribe = Store.subscribe(store);
  let dispatch = Store.dispatch(store);

  let updater = (state, action) => {
    let (newState, effect) = Config.updater(state, action);
    Effect.run(effect, dispatch);

    newState;
  };

  Store.replaceReducer(store, updater);

  Effect.run(initEffect, dispatch);

  let useSelector = selector => {
    let getCurrentValue = () => selector(Store.getState(store));
    let selectedState =
      Subscription.useSubscription(subscribe, getCurrentValue);

    selectedState;
  };
};
