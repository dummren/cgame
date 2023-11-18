#include "sfx.h"

#include "audio.h"
#include "res.h"

cg_audio_source_t cgSfxBeepSource;
cg_audio_source_t cgSfxAcceptSource;

static cg_audio_source_t f_load(const char *path) {
  unsigned char data[cgResLen(path)];
  cgResRead(path, data);

  cg_audio_source_t source = cgAudioSource();
  cgAudioSourceLoad(source, data, cgResLen(path));

  return source;
}

void cgSfxInit() {
  cgSfxBeepSource = f_load("sfx/beep.ogg");
  cgSfxAcceptSource = f_load("sfx/accept.ogg");
}

void cgSfxBeep() {
  cgAudioSourcePlay(cgSfxBeepSource);
}

void cgSfxAccept() {
  cgAudioSourcePlay(cgSfxAcceptSource);
}
