open Revery;
open Revery.UI;
open FocusTimer_Models;

let make = (~time: Time.t, ~currentTimer, ()) => {
  let text =
    time |> Time.toFloatSeconds |> TimeInterval.make |> TimeInterval.format;

  <View>
    <Text
      style=Style.[
        fontFamily("Roboto-Regular.ttf"),
        fontSize(54),
        marginVertical(20),
        color(currentTimer |> Theme.colorFromTimer(`Text)),
      ]
      text
    />
  </View>;
};
