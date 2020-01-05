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
      backgroundColor((isHovered ? `ButtonHover : `Button) |> Theme.color),
      border(~width=2, ~color=`ButtonBorder |> Theme.color),
      borderRadius(2.0),
      flexGrow(0),
      minWidth(120),
    ];

  let (size, line) = Theme.fontVariant(`base);
  let text =
    Style.[
      color(`ButtonText |> Theme.color),
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
