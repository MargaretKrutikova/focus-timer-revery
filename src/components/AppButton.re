open Revery;
open Revery.UI;
open Revery.UI.Components;
open FocusTimer_Utils.Utils;

module Styles = {
  let wrapperStyle = (~isHovered, ~isDisabled) => {
    let bg =
      switch (isHovered, isDisabled) {
      | (true, false) => `ButtonHover |> Theme.color
      | (false, false) => `Button |> Theme.color
      | (_, true) => `Button |> Theme.color |> Color.multiplyAlpha(0.5)
      };

    Style.[
      position(`Relative),
      justifyContent(`Center),
      alignItems(`Center),
      paddingVertical(`sm |> Theme.space),
      paddingHorizontal(`md |> Theme.space),
      backgroundColor(bg),
      border(~width=2, ~color=`ButtonBorder |> Theme.color),
      borderRadius(2.0),
      flexGrow(0),
      minWidth(120),
      cursor(isDisabled ? MouseCursors.arrow : MouseCursors.pointer),
    ];
  };

  let (size, line) = Theme.fontVariant(`base);
  let text = (~isDisabled) => {
    let textColor =
      `ButtonText
      |> Theme.color
      |> (isDisabled ? Color.multiplyAlpha(0.5) : identity);

    Style.[
      color(textColor),
      textWrap(TextWrapping.NoWrap),
      fontSize(size),
      lineHeight(line),
      fontFamily("Roboto-Regular.ttf"),
    ];
  };
};

let%component make = (~label, ~disabled=false, ~onClick=?, ()) => {
  let%hook (isHovered, setHovered) = Hooks.state(false);

  <Clickable ?onClick>
    <View
      style={Styles.wrapperStyle(~isHovered, ~isDisabled=disabled)}
      onMouseOver={_ => setHovered(_wasHovered => true)}
      onMouseOut={_ => setHovered(_wasHovered => false)}>
      <Text style={Styles.text(~isDisabled=disabled)} text=label />
    </View>
  </Clickable>;
};
