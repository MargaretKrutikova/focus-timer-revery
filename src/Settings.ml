
type t = {
  focusDuration: float;
  restDuration: float;
  defaultTimer: TimerModel.t
}

let getDuration settings timer =
  match timer with
  | TimerModel.Focus -> settings.focusDuration
  | Rest -> settings.restDuration
