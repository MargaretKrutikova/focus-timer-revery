#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>

#include "simple-sdl2-audio.h"

CAMLprim value SDL_initAudioCAML();
CAMLprim value SDL_endAudioCAML();
CAMLprim value SDL_playSoundCAML(value name, value volumeScale);
