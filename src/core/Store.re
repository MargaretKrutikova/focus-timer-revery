type reducer('action, 'state) =
  ('state, 'action) => ('state, Effect.t('action));

type dispatch('action) = 'action => unit;

type listener = unit => unit;

type t('action, 'state) = {
  state: ref('state),
  reducer: ref(reducer('action, 'state)),
  listeners: ref(list(listener)),
  effects: ref(list(Effect.t('action))),
  customDispatcher:
    option((t('action, 'state), dispatch('action), 'action) => unit),
};

type middleware('action, 'state) =
  (t('action, 'state), 'action => unit, 'action) => unit;

let create = (~reducer, ~preloadedState, ~enhancer=?, ()) => {
  state: ref(preloadedState),
  listeners: ref([]),
  effects: ref([]),
  reducer: ref(reducer),
  customDispatcher: enhancer,
};

let unsubscribe = (store, listener, ()) =>
  store.listeners := List.filter(l => listener !== l, store.listeners^);

let subscribe = (store, listener) => {
  store.listeners := [listener, ...store.listeners^];
  unsubscribe(store, listener);
};

let queueEffect = (store, effect) => {
  store.effects := [effect, ...store.effects^];
};

let nativeDispatch = (store: t('a, 's), action: 'a) => {
  let (state, effect) = store.reducer^(store.state^, action);

  store.state := state;
  queueEffect(store, effect);

  List.iter(listener => listener(), store.listeners^);
};

let dispatch = (store, action) =>
  switch (store.customDispatcher) {
  | Some(customDispatcher) =>
    customDispatcher(store, nativeDispatch(store), action)
  | None => nativeDispatch(store, action)
  };

let runEffects = store => {
  store.effects^ |> List.iter(effect => Effect.run(effect, dispatch(store)));
  store.effects := [];
};

let getState = store => store.state^;
let getEffects = store => store.effects^;

let replaceReducer = (store, reducer) => store.reducer := reducer;
