type todo = unit

type notStartedData = todo

type stoppedData = todo

type timerRunningData =
    { started: float;
      elapsed: float; }

type timerPausedData =
    { started: float;
      elapsed: float;
      paused: float; }

type timer = Focus | Rest

type settings = {
  focusDuration: float;
  restDuration: float;
}

type state =
    | StoppedState of stoppedData
    | TimerRunningState of (timer * timerRunningData)
    | TimerPausedState of (timer * timerPausedData)

type focusTimerAction =
    | FocusTimeElapse
    | RestTimeElapse
    | Tick of float
    | TimerStarted
    | TimerPaused
    | TimerResumed
    | TimerStopped

let getDuration settings timer =
  match timer with
  | Focus -> settings.focusDuration
  | Rest -> settings.restDuration

let shouldRestartTimer prevState nextState = 
  match prevState, nextState with
  | TimerRunningState (prevTimer, _), TimerRunningState (nextTimer, _) ->
    prevTimer != nextTimer
  | _, _ -> true

let nextTimer = 
  function
  | Focus -> Rest
  | Rest -> Focus

let startTimer clock timer : state =
  TimerRunningState (timer, { started=clock (); elapsed=0.0})

let pauseTimer clock timer (tr: timerRunningData) : state  =
  TimerPausedState (timer, { started = tr.started; elapsed=tr.elapsed; paused = clock() })

let nextTick duration clock timer (tr: timerRunningData) tick : state =
  let elapsed = tr.elapsed +. tick in
  if elapsed > duration
  then timer |> nextTimer |> startTimer clock
  else TimerRunningState (timer, { tr with elapsed=elapsed})

let resumeFocusTimer clock shouldRestartFocusTimer (ftp: timerPausedData) : state =
  if shouldRestartFocusTimer ftp
  then Focus |> nextTimer |> startTimer clock
  else TimerRunningState (Focus, { started=clock (); elapsed=ftp.elapsed})

let resumeRestTimer clock shouldResumeRestTimer (rtp: timerPausedData) : state =
  if shouldResumeRestTimer rtp
  then TimerRunningState (Rest, { started = clock(); elapsed=rtp.elapsed })
  else Rest |> nextTimer |> startTimer clock

module StateMachine = struct
    let run ~shouldRestartFocusTimer ~shouldResumeRestTimer ~settings ~clock action state : state = 
      match action with
      | TimerStopped -> StoppedState()
      | TimerStarted -> 
        (match state with
        | StoppedState _ -> startTimer clock Focus
        | _ -> state)
      | FocusTimeElapse ->
        (match state with
        | TimerRunningState (Focus, ftr) -> Focus |> nextTimer |> startTimer clock
        | _ -> state)
      | RestTimeElapse ->
        (match state with
        | TimerRunningState (Rest, rtr) -> Rest |> nextTimer |> startTimer clock
        | _ -> state)
      | TimerResumed ->
        (match state with 
        | TimerPausedState (Focus, tp) -> resumeFocusTimer clock shouldRestartFocusTimer tp
        | TimerPausedState (Rest, tp) -> resumeRestTimer clock shouldResumeRestTimer tp
        | _ -> state)
      | TimerPaused -> 
        (match state with 
        | TimerRunningState (timer, tr) -> pauseTimer clock timer tr
        | _ -> state)
      | Tick tick -> 
        (match state with 
        | TimerRunningState (timer, tr) -> 
          nextTick (getDuration settings timer) clock timer tr tick
        | _ -> state)
  end
