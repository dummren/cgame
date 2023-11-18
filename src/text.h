#ifndef __CG__TEXT_H__
#define __CG__TEXT_H__

#include "cglm/struct.h"
#include "stb/stb_truetype.h"

typedef struct {
  stbtt_fontinfo *info;
} cg_text_font_t;

typedef enum {
  CG_TEXT_ALIGN_LEFT,
  CG_TEXT_ALIGN_CENTER,
  CG_TEXT_ALIGN_RIGHT,
} cg_text_align_t;

typedef enum {
  CG_TEXT_VALIGN_TOP,
  CG_TEXT_VALIGN_MIDDLE,
  CG_TEXT_VALIGN_BOTTOM,
} cg_text_valign_t;

void cgTextInit();

cg_text_font_t *cgTextFont(unsigned char*);

ivec2s cgTextCharSize(const cg_text_font_t*, const char, const int);
ivec2s cgTextStrSize(const cg_text_font_t*, const char*, const int);

void cgTextFontBind(cg_text_font_t*);
void cgTextFontUnbind();

cg_text_font_t *cgTextFontCurrent();

void cgTextReset();

void cgTextAlign(const cg_text_align_t);
void cgTextValign(const cg_text_valign_t);
void cgTextColor(const float, const float, const float, const float);

void cgTextDraw(const float, const float, const float, const char*, const int);

#endif // __CG__TEXT_H__
