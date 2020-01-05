open Revery.Color
open Revery.UI
open FocusTimer_Models

(* base values *)
let baseFontSize = 20
let baseLineHeight = 1.4
let headingLineHeight = 1.2

let baseLineGrid = 4
let fontScale = 1.5

module Colors = struct 
  let darkGreen = hex "#004643"
  let darkerGreen = hex "#0f3433"
  let lightGreen = hex "#abd1c6"

  let yellow = hex "#f9bc60"
  let lightYellow = hex "#FBD499"
  let darkerYellow = hex "#F7A427"

  let pink = hex "#e16162"
  let lightPink = hex "#F9DFDF"
  let darkerPink = hex "#6A1515"

  let white = hex "#fffffe"
  let black = hex "#001e1d"
  let lightGray = hex "#e8e4e6"
end

(* utils *)
let fontSize factor =
  fontScale ** (factor |> float_of_int)
  *. (baseFontSize |> float_of_int)
  |> int_of_float

let lineHeight = function
  | `body -> baseLineHeight
  | `heading -> headingLineHeight

(* tokens *)

let fontVariant variant : int * float = 
  let genVariant factor line = (fontSize factor, lineHeight line) in

  match variant with
  | `xs -> genVariant (-2) `body
  | `sm -> genVariant (-1) `body
  | `base -> genVariant 0 `body
  | `md -> genVariant 1 `body
  | `lg -> genVariant 2 `heading
  | `xl -> genVariant 3 `heading
  | `xxl -> genVariant 4 `heading

let space = function
  | `xxs -> baseLineGrid
  | `xs -> baseLineGrid * 2
  | `sm -> baseLineGrid * 3
  | `md -> baseLineGrid * 4
  | `lg -> baseLineGrid * 6
  | `xl -> baseLineGrid * 8
  | `xxl -> baseLineGrid * 10
  | `custom multiplier -> multiplier * baseLineGrid

let color = function 
  | `Bg -> Colors.lightGreen
  | `ButtonText -> Colors.black
  | `Button -> Colors.yellow
  | `ButtonHover -> Colors.darkerYellow
  | `ButtonBorder -> Colors.black

type timerColor = [ `Bg | `Text | `Primary ]

let colorFromTimer color timer =
  match timer with
  | TimerModel.Focus -> 
    (match color with
    | `Bg -> Colors.lightPink
    | `Text -> Colors.darkerPink
    | `Primary -> Colors.darkerPink)
  | TimerModel.Rest -> 
    (match color with
    | `Bg -> Colors.lightGreen
    | `Text -> Colors.darkerGreen
    | `Primary -> Colors.darkerGreen)

(* styles *)
let textStyle variant : Style.textStyleProps list=
  let size, line = fontVariant variant in
  Style.[fontSize size; lineHeight line] 

let fontFamily ?weight:(w=`Regular) () = 
  match w with
  | `Regular -> "Montserrat-Regular.ttf"

