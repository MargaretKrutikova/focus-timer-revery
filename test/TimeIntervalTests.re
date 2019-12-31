open TestFramework;
open FocusTimer_Utils;

describe("Time interval utils", ({test}) => {
  test("formats time correctly", ({expect}) => {
    let time: TimeInterval.t = {hour: 0, min: 0, sec: 10};
    expect.string(TimeInterval.format(time)).toEqual("0:10");
  })
});
