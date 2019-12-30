open CoreTypes;

type t('action, 'state) = {
  state: ref('state),
  reducer: ref(reducer('action, 'state)),
  listeners: ref(list(listener)),
  customDispatcher:
    option((t('action, 'state), dispatch('action), 'action) => unit),
};

type middleware('action, 'state) =
  (t('action, 'state), 'action => unit, 'action) => unit;

let create = (~reducer, ~preloadedState, ~enhancer=?, ()) => {
  state: ref(preloadedState),
  listeners: ref([]),
  reducer: ref(reducer),
  customDispatcher: enhancer,
};

let unsubscribe = (store, listener, ()) =>
  store.listeners := List.filter(l => listener !== l, store.listeners^);

let subscribe = (store, listener) => {
  store.listeners := [listener, ...store.listeners^];
  unsubscribe(store, listener);
};

let nativeDispatch = (store, action) => {
  store.state := store.reducer^(store.state^, action);
  List.iter(listener => listener(), store.listeners^);
};

let dispatch = (store, action) =>
  switch (store.customDispatcher) {
  | Some(customDispatcher) =>
    customDispatcher(store, nativeDispatch(store), action)
  | None => nativeDispatch(store, action)
  };

let getState = store => store.state^;
let replaceReducer = (store, reducer) => store.reducer := reducer;
