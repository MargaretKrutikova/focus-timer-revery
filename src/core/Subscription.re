open Revery.UI;

type source('a) = {
  getCurrentValue: unit => 'a,
  value: 'a,
};

type action('a) =
  | StateUpdated(source('a))
  | ValueUpdated('a);

let reducer = (action, state) => {
  switch (action) {
  | StateUpdated(s) => s
  | ValueUpdated(value) => state.value !== value ? {...state, value} : state
  };
};

let useSubscription =
    (subscribe: (unit => unit, unit) => unit, getCurrentValue: unit => 'a) => {
  let hasSubscriptionChanged = prevState =>
    prevState.getCurrentValue !== getCurrentValue;

  let%hook (state, dispatch) =
    Hooks.reducer(
      ~initialState={getCurrentValue, value: getCurrentValue()},
      reducer,
    );

  let%hook () =
    Hooks.effect(
      OnMount,
      () => {
        let didUnsubscribe = ref(false);

        let updateState = () => {
          let newValue = getCurrentValue();
          dispatch(ValueUpdated(newValue));
        };

        let checkForUpdates = () =>
          if (! didUnsubscribe^) {
            updateState();
          };

        // state value might have changed between unscribing and subscribing again due
        // to a new selector function (e.g. because of a change in props used inside of it).
        updateState();

        let unsubscribe = subscribe(checkForUpdates);

        Some(
          () => {
            didUnsubscribe := true;
            unsubscribe();
          },
        );
      },
    );

  // evaluate the return value
  if (hasSubscriptionChanged(state)) {
    let newValue = getCurrentValue();
    dispatch(StateUpdated({getCurrentValue, value: newValue}));

    newValue;
  } else {
    state.value;
  };
};
