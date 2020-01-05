open FocusTimer_Models;

type dispose = unit => unit;
type clock = unit => float;

module TimerRunningData = {
  type t = {
    dispose,
    started: float,
    elapsed: float,
    timer: TimerModel.t,
  };
  let make = (~started, ~elapsed, dispose, timer) => {
    dispose,
    elapsed,
    started,
    timer,
  };
  let setElapsed = (elapsed, data) => {...data, elapsed};
};

module TimerPausedData = {
  type t = {
    started: float,
    elapsed: float,
    paused: float,
    timer: TimerModel.t,
  };

  let make = (~started, ~elapsed, ~paused, timer) => {
    started,
    elapsed,
    paused,
    timer,
  };
};

module TimerScheduledData = {
  type type_ =
    | NewTimer
    | ResumedTimer(float);
  type t = {
    timer: TimerModel.t,
    type_,
  };

  let make = (type_, timer) => {timer, type_};
  let toElapsed = data =>
    switch (data.type_) {
    | NewTimer => 0.0001 // simulate already running timer
    | ResumedTimer(elapsed) => elapsed
    };
};

type state =
  | Idle(TimerModel.t)
  | TimerRunningState(TimerRunningData.t)
  | TimerScheduledState(TimerScheduledData.t)
  | TimerPausedState(TimerPausedData.t);

let initialState = timer => Idle(timer);

let isStateEqual = (expect, actual) => compare(expect, actual) == 0;

let currentTimer =
  fun
  | Idle(timer) => timer
  | TimerRunningState({timer, _})
  | TimerScheduledState({timer, _})
  | TimerPausedState({timer, _}) => timer;

let isIdle =
  fun
  | Idle(_) => true
  | _ => false;
