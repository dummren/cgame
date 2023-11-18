#ifndef __CG__SFX_H__
#define __CG__SFX_H__

#include "audio.h"

extern cg_audio_source_t cgSfxBeepSource;
extern cg_audio_source_t cgSfxAcceptSource;

void cgSfxInit();

void cgSfxBeep();
void cgSfxAccept();

#endif // __CG__SFX_H__
