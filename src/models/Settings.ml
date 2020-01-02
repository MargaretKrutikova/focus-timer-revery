type t = {
  focusDurationSec: float;
  restDurationSec: float;
  defaultTimer: TimerModel.t;
  focusTimerElapseSoundPath: string;
  restTimerElapseSoundPath: string
}

let getDuration settings timer =
  match timer with
  | TimerModel.Focus -> settings.focusDurationSec
  | Rest -> settings.restDurationSec

let getTimerElapseSound settings timer =
  match timer with
  | TimerModel.Focus -> settings.focusTimerElapseSoundPath
  | Rest -> settings.restTimerElapseSoundPath
