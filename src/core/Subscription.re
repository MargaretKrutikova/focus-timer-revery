open Revery;
open Revery.UI;

type source('a) = {
  getCurrentValue: unit => 'a,
  value: 'a,
};

let useSubscription =
    (subscribe: (unit => unit, unit) => unit, getCurrentValue: unit => 'a) => {
  let hasSubscriptionChanged = prevState =>
    prevState.getCurrentValue !== getCurrentValue;

  let%hook (state, setState) =
    Hooks.state({getCurrentValue, value: getCurrentValue()});

  let%hook () =
    Hooks.effect(
      OnMount,
      () => {
        let didUnsubscribe = ref(false);

        let updateState = () =>
          setState(prevState =>
            if (hasSubscriptionChanged(prevState)) {
              prevState;
            } else {
              let newValue = getCurrentValue();
              prevState.value === newValue
                ? prevState : {...prevState, value: newValue};
            }
          );

        let checkForUpdates = () =>
          if (! didUnsubscribe^) {
            updateState();
          };

        // state value might have changed between unscribing and subscribing again due
        // to a new selector function (e.g. because of a change in props used inside of it).
        let newValue = getCurrentValue();
        if (state.value !== newValue) {
          setState(prevState => {...prevState, value: newValue});
        };

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
    setState(_ => {getCurrentValue, value: newValue});

    newValue;
  } else {
    state.value;
  };
};
