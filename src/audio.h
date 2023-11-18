#ifndef __CG__AUDIO_H__
#define __CG__AUDIO_H__

#include <stdbool.h>

typedef unsigned int cg_audio_source_t;

void cgAudioInit();
void cgAudioListenReset();

void cgAudioListenPos(const float, const float, const float);
void cgAudioListenVel(const float, const float, const float);
void cgAudioListenOri(const float, const float, const float,
                      const float, const float, const float);
void cgAudioDistMdl(const unsigned int);

cg_audio_source_t cgAudioSource();
void cgAudioSourceReset(cg_audio_source_t);

void cgAudioSourcePos(cg_audio_source_t,
                      const float, const float, const float);
void cgAudioSourceVel(cg_audio_source_t,
                      const float, const float, const float);
void cgAudioSourcePitch(cg_audio_source_t, const float);
void cgAudioSourceGain(cg_audio_source_t, const float);
void cgAudioSourceLoop(cg_audio_source_t, const bool);
void cgAudioSourceMaxDist(cg_audio_source_t, const float);

// only supports 16 bit pcm
void cgAudioSourceLoad(cg_audio_source_t,
                       const unsigned char*,
                       const int);
void cgAudioSourcePlay(cg_audio_source_t);

#endif // __CG__AUDIO_H__
