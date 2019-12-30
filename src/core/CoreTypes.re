type dispatch('action) = 'action => unit;

type reducer('action, 'state) = ('state, 'action) => 'state;

type listener = unit => unit;
