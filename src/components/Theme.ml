open Revery.Color
open Revery.UI

(* base values *)
let baseFontSize = 20
let baseLineHeight = 1.4
let headingLineHeight = 1.2

let baseLineGrid = 4
let fontScale = 1.2

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
  | `Bg -> rgb 53.0 61.0 74.0
  | `PrimaryText -> rgb 200.0 200.0 200.0

(* styles *)
let textStyle variant : Style.textStyleProps list=
  let size, line = fontVariant variant in
  Style.[fontSize size; lineHeight line] 
