open CoreTypes;

type effect('action) = {fn: dispatch('action) => unit};

type t('action) = option(effect('action));

let create = (fn: dispatch('action) => unit) => Some({fn: fn});

let none: t('action) = None;

let run = (effect: t('action), dispatch: dispatch('action)) => {
  switch (effect) {
  | None => ()
  | Some(effect) => effect.fn(dispatch)
  };
};

let batch = (effects: list(t('a))) => {
  let effects = effects |> List.filter(e => e != None);

  let execute = dispatch => {
    List.iter(e => run(e, dispatch), effects);
  };

  switch (effects) {
  | [] => None
  | _ => Some({fn: execute})
  };
};
