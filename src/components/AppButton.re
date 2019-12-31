open Revery;
open Revery.UI;
open Revery.UI.Components;

module Styles = {
  let wrapperStyle = (~isHovered) =>
    Style.[
      position(`Relative),
      justifyContent(`Center),
      alignItems(`Center),
      paddingVertical(`sm |> Theme.space),
      paddingHorizontal(`md |> Theme.space),
      border(~width=1, ~color=isHovered ? Colors.lightBlue : Colors.blue),
      borderRadius(2.),
    ];

  let (size, line) = Theme.fontVariant(`base);
  let text =
    Style.[
      color(`PrimaryText |> Theme.color),
      textWrap(TextWrapping.NoWrap),
      fontSize(size),
      lineHeight(line),
      fontFamily("Roboto-Regular.ttf"),
    ];
};

let%component make = (~label, ~onClick=?, ()) => {
  let%hook (isHovered, setHovered) = Hooks.state(false);

  <Clickable ?onClick>
    <View
      style={Styles.wrapperStyle(~isHovered)}
      onMouseOver={_ => setHovered(_wasHovered => true)}
      onMouseOut={_ => setHovered(_wasHovered => false)}>
      <Text style=Styles.text text=label />
    </View>
  </Clickable>;
};
