#include <SDL2/SDL.h>
#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>

int play_wav(char * filename);
Uint8 * adjust_volume(Uint8 * srcBuffer, int length, SDL_AudioFormat format);

CAMLprim value initAudioCAML();
CAMLprim value playWavCAML(value vFile);
