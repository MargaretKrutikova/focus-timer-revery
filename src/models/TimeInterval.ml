type t = {
  sec: int;
  min: int;
  hour: int;
}

type division = {
  remainder: int;
  quotient: int;
}

let divide x y = 
  {remainder = x mod y; quotient = x/y}

let make (seconds: float) =
  let hour = divide (seconds |> int_of_float) (60 * 60) in
  let min = divide hour.remainder 60 in

  {hour= hour.quotient;min = min.quotient; sec = min.remainder }

let prependZero value =
  if value < 10 then
    value |> string_of_int |> ((^) "0")
  else
    value |> string_of_int

let format {hour; min; sec} = 
  let secStr = prependZero sec in
  if hour = 0 then
    Printf.sprintf "%d:%s" min secStr
  else 
    Printf.sprintf "%d:%d:%s" hour min secStr

