#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <caml/threads.h>

#include <SDL2/SDL.h>

#include "simple-sdl2-audio.h"
#include "audio.h"


CAMLprim value SDL_initAudioCAML() {
  CAMLparam0();

  // Initialize SDL2 Audio only
  SDL_Init(SDL_INIT_AUDIO);

  // Initialize Simple-SDL2-Audio
  initAudio();

  CAMLreturn(Val_unit);
}

CAMLprim value SDL_endAudioCAML() {
  CAMLparam0();

  // End Simple-SDL2-Audio
  endAudio();

  // End SDL2
  SDL_Quit();

  CAMLreturn(Val_unit);
}

CAMLprim value SDL_playSoundCAML(value name, value volumeScale) {
  CAMLparam2(name, volumeScale);

  const char *filename = String_val(name);

  // the max sound is horribly loud, so adjust it by half
  double volume = (SDL_MIX_MAXVOLUME * Double_val(volumeScale)) / 2;
  caml_release_runtime_system();

  playSound(filename, volume);

  caml_acquire_runtime_system();

  SDL_Delay(1000);
  CAMLreturn(Val_unit);
}
