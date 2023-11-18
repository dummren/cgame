#include "audio.h"

#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#include "stb/stb_vorbis.c"

#include "alcore.h"
#include "settings.h"

static ALCdevice *f_device;
static ALCcontext *f_context;

void cgAudioInit() {
  f_device = alcOpenDevice(NULL);

  if (!f_device)
    puts("failed to open audio device");

  f_context = alcCreateContext(f_device, NULL);

  if (!alcMakeContextCurrent(f_context))
    puts("failed to make audio device context current");
}

void cgAudioListenReset() {
  cgAudioListenPos(0.0f, 0.0f, 0.0f);
  cgAudioListenVel(0.0f, 0.0f, 0.0f);
  cgAudioListenOri(0.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 0.0f);
  cgAudioDistMdl(AL_LINEAR_DISTANCE_CLAMPED);
}

void cgAudioListenPos(const float x, const float y, const float z) {
  alListener3f(AL_POSITION, x, y, z);
}

void cgAudioListenVel(const float x, const float y, const float z) {
  alListener3f(AL_VELOCITY, x, y, z);
}

void cgAudioListenOri(const float fx, const float fy, const float fz,
                      const float ux, const float uy, const float uz) {
  const float orientation[] = {
    fx, fy, fz,
    ux, uy, uz,
  };

  alListenerfv(AL_ORIENTATION, orientation);
}

void cgAudioDistMdl(const unsigned int value) {
  alDistanceModel(value);
}

cg_audio_source_t cgAudioSource() {
  cg_audio_source_t src;
  alGenSources(1, &src);
  return src;
}

void cgAudioSourceReset(cg_audio_source_t source) {
  cgAudioSourcePos(source, 0.0f, 0.0f, 0.0f);
  cgAudioSourceVel(source, 0.0f, 0.0f, 0.0f);
  cgAudioSourcePitch(source, 1.0f);
  cgAudioSourceGain(source, 1.0f);
  cgAudioSourceLoop(source, false);
  cgAudioSourceMaxDist(source, 100.0f);
}

void cgAudioSourcePos(cg_audio_source_t source,
                      const float x, const float y, const float z) {
  alSource3f(source, AL_POSITION, x, y, z);
}

void cgAudioSourceVel(cg_audio_source_t source,
                      const float x, const float y, const float z) {
  alSource3f(source, AL_VELOCITY, x, y, z);
}

void cgAudioSourcePitch(cg_audio_source_t source, const float value) {
  alSourcef(source, AL_PITCH, value);
}

void cgAudioSourceGain(cg_audio_source_t source, const float value) {
  alSourcef(source, AL_GAIN, value);
}

void cgAudioSourceLoop(cg_audio_source_t source, const bool value) {
  alSourcef(source, AL_LOOPING, value);
}

void cgAudioSourceMaxDist(cg_audio_source_t source, const float value) {
  alSourcef(source, AL_MAX_DISTANCE, value);
}

void cgAudioSourceLoad(cg_audio_source_t source,
                       const unsigned char *data,
                       const int len) {
  unsigned int buffer;
  alGenBuffers(1, &buffer);

  int channels, sampleRate;
  short *bufferData;
  int samples = stb_vorbis_decode_memory(data, len,
                                         &channels, &sampleRate, &bufferData);

  if (samples < 0)
    puts("failed to load audio track");

  samples *= 2 * channels;
  int format = (channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;

  alBufferData(buffer, format, bufferData, samples, sampleRate);
  alSourcei(source, AL_BUFFER, buffer);
}

typedef struct {
  cg_audio_source_t source;
} f_play_thread_args_t;

static void *f_playThread(void *args) {
  f_play_thread_args_t *argsStruct = args;

  int state;
  alSourcePlay(argsStruct->source);

  do {
    alSourcef(argsStruct->source, AL_GAIN, CGS_GAIN);
    alGetSourcei(argsStruct->source, AL_SOURCE_STATE, &state);
  } while (state == AL_PLAYING);

  return NULL;
}

void cgAudioSourcePlay(cg_audio_source_t source) {
  f_play_thread_args_t *args = malloc(sizeof(f_play_thread_args_t));
  args->source = source;

  pthread_t playThread;
  pthread_create(&playThread, NULL, &f_playThread, args);
}
