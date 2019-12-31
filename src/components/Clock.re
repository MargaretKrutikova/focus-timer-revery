open Revery;
open Revery.UI;
open FocusTimer_Utils;

module Interval = {
  let make = (~time: Time.t, ()) => {
    let text =
      time |> Time.toFloatSeconds |> TimeInterval.make |> TimeInterval.format;
    <View>
      <Text
        style=Style.[
          color(Colors.white),
          fontFamily("Roboto-Regular.ttf"),
          fontSize(54),
          marginVertical(20),
        ]
        text
      />
    </View>;
  };
};
