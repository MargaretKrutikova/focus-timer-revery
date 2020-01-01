external endSound: unit -> unit = "SDL_endAudioCAML"

external initSound: unit -> unit = "SDL_initAudioCAML"

external playSound: string -> float -> unit = "SDL_playSoundCAML"

let playSound volume path =
  Thread.create (fun () -> playSound path volume) ()
