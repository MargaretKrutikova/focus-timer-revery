(* Long break timer can be added later *)
type t = Focus | Rest

let next = 
  function
  | Focus -> Rest
  | Rest -> Focus
