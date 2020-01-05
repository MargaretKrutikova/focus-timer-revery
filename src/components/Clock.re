open Revery;
open Revery.UI;
open Revery.Font;
open FocusTimer_Models;

let make = (~time: Time.t, ~currentTimer, ()) => {
  let text =
    time |> Time.toFloatSeconds |> TimeInterval.make |> TimeInterval.format;
  let (size, line) = Theme.fontVariant(`xxl);

  <View>
    <Text
      style=Style.[
        fontFamily(Theme.fontFamily()),
        fontSize(size),
        lineHeight(line),
        color(currentTimer |> Theme.colorFromTimer(`Text)),
      ]
      text
    />
  </View>;
};
