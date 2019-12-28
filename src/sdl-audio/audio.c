#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <caml/threads.h>

#include <SDL2/SDL.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "audio.h"

int play_wav(char * filename) 
{
  SDL_AudioSpec wavSpec;
  Uint32 wavLength;
  Uint8 * wavBuffer;
 
  /* Load the WAV */
  if (SDL_LoadWAV(filename, &wavSpec, &wavBuffer, &wavLength) == NULL) 
  {
      fprintf(stderr, "[%s: %d]Warning: failed to open wave file: %s error: %s\n", __FILE__, __LINE__, filename, SDL_GetError());
      return -1;
  }

  /* Adjust volume */
  Uint8 * wavBufferVolume = adjust_volume(wavBuffer, wavLength, wavSpec.format);

  SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
  SDL_QueueAudio(deviceId, wavBufferVolume, wavLength);

  SDL_PauseAudioDevice(deviceId, 0);

  SDL_Delay(3000);
  
  SDL_CloseAudioDevice(deviceId);
 
  SDL_FreeWAV(wavBuffer);
  SDL_FreeWAV(wavBufferVolume);

  return 0;
}

CAMLprim value initAudioCAML() {
  CAMLparam0();

  SDL_Init(SDL_INIT_AUDIO);
  
  CAMLreturn(Val_unit);
}

Uint8 * adjust_volume(Uint8 * srcBuffer, int length, SDL_AudioFormat format)
{
    Uint8 * mixBuffer = malloc(length);

    // make sure this is silence
    SDL_memset(mixBuffer, 0, length);  

    // mix the audio against the silence, at 50% volume.
    SDL_MixAudioFormat(mixBuffer, srcBuffer, format, length , SDL_MIX_MAXVOLUME / 20);
    
    return mixBuffer;
}

CAMLprim value playWavCAML(value vFile) {
  CAMLparam1(vFile);

  caml_release_runtime_system();

  int success = play_wav(String_val(vFile));

  caml_acquire_runtime_system();

  CAMLreturn(Val_int(success));
}
