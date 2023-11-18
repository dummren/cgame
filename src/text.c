#include "text.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stddef.h>

#include "cglm/struct.h"
#include "stb/stb_truetype.h"

#include "glcore.h"
#include "texture.h"
#include "shader.h"
#include "mesh.h"
#include "main.h"
#include "shaders.h"
#include "settings.h"

static const unsigned int f_CG_TEXT_MESH_INDICES[] = {
  0, 1, 2,
  2, 3, 0,
};

static const float f_CG_TEXT_MESH_POSITIONS[] = {
  0.5f, 0.5f, 0.0f,
  -0.5f, 0.5f, 0.0f,
  -0.5f, -0.5f, 0.0f,
  0.5f, -0.5f, 0.0f,
};

static const float f_CG_TEXT_MESH_UV[] = {
  1.0f, 0.0f,
  0.0f, 0.0f,
  0.0f, 1.0f,
  1.0f, 1.0f,
};

static cg_shader_prog_t f_cgTextProg;
static cg_mesh_t *f_cgTextMesh;

static cg_text_font_t *f_cgTextCurrentFont = NULL;
static cg_text_align_t f_cgTextCurrentAlign;
static cg_text_valign_t f_cgTextCurrentValign;
static vec4s f_cgTextCurrentColor;

void cgTextInit() {
  cgTextReset();

  f_cgTextProg = cgShadersTextProg;
  f_cgTextMesh = cgMesh();

  cgMeshStoreIndices(f_cgTextMesh,
                     f_CG_TEXT_MESH_INDICES,
                     sizeof(f_CG_TEXT_MESH_INDICES),
                     0);
  cgMeshStoref(f_cgTextMesh,
               f_CG_TEXT_MESH_POSITIONS,
               sizeof(f_CG_TEXT_MESH_POSITIONS),
               2, 3);
  cgMeshStoref(f_cgTextMesh,
               f_CG_TEXT_MESH_UV,
               sizeof(f_CG_TEXT_MESH_UV),
               3, 2);
}

cg_text_font_t *cgTextFont(unsigned char *data) {
  cg_text_font_t *font = malloc(sizeof(cg_text_font_t));

  font->info = malloc(sizeof(stbtt_fontinfo));

  if (!stbtt_InitFont(font->info,
                      data,
                      stbtt_GetFontOffsetForIndex(data, 0))) {
    puts("failed to load font data");
    return NULL;
  }

  return font;
}

ivec2s cgTextCharSize(const cg_text_font_t *font,
                      const char chr,
                      const int fontSize) {
  float scale = stbtt_ScaleForPixelHeight(font->info, fontSize);
  ivec2s size;

  int x1, y1, x2, y2;
  stbtt_GetCodepointBitmapBox(font->info, chr,
                              scale, scale,
                              &x1, &y1,
                              &x2, &y2);

  size.x = x2 - x1;
  size.y = y2 - y1;

  return size;
}

ivec2s cgTextStrSize(const cg_text_font_t *font,
                     const char *str,
                     const int fontSize) {
  float scale = stbtt_ScaleForPixelHeight(font->info, fontSize);

  ivec2s size;
  size.x = 0;
  size.y = 0;

  int currentLineWidth = 0;

  for (size_t i = 0; i < strlen(str); i++) {
    const char c = str[i];

    if (c == '\n') {
      if (currentLineWidth > size.x)
        size.x = currentLineWidth;

      size.y += fontSize;
      currentLineWidth = 0;

      continue;
    }

    int advance, leftSideBearing;
    stbtt_GetCodepointHMetrics(font->info, c, &advance, &leftSideBearing);

    advance *= scale;
    leftSideBearing *= scale;

    int x1, y1, x2, y2;
    stbtt_GetCodepointBitmapBox(font->info, c,
                                scale, scale,
                                &x1, &y1,
                                &x2, &y2);

    currentLineWidth += advance;

    if (i == strlen(str) - 1) {
      if (currentLineWidth > size.x)
        size.x = currentLineWidth;

      size.y += fontSize;
      currentLineWidth = 0;
    }
  }

  return size;
}

void cgTextFontBind(cg_text_font_t *font) {
  f_cgTextCurrentFont = font;
}

void cgTextFontUnbind() {
  f_cgTextCurrentFont = NULL;
}

cg_text_font_t *cgTextFontCurrent() {
  return f_cgTextCurrentFont;
}

void cgTextReset() {
  f_cgTextCurrentAlign = CG_TEXT_ALIGN_LEFT;
  f_cgTextCurrentValign = CG_TEXT_VALIGN_TOP;
  f_cgTextCurrentColor = (vec4s) { 1.0f, 1.0f, 1.0f, 1.0f };
}

void cgTextAlign(const cg_text_align_t align) {
  f_cgTextCurrentAlign = align;
}

void cgTextValign(const cg_text_valign_t valign) {
  f_cgTextCurrentValign = valign;
}

void cgTextColor(const float r,
                    const float g,
                    const float b,
                    const float a) {
  f_cgTextCurrentColor = (vec4s) { r, g, b, a };
}

void cgTextDraw(const float x,
                const float y,
                const float z,
                const char *str,
                const int fontSize) {
  cg_text_font_t *font = f_cgTextCurrentFont;

  if (!font)
    return;

  float scale = stbtt_ScaleForPixelHeight(font->info, fontSize);

  cgShaderProgBind(f_cgTextProg);
  cgMeshBind(f_cgTextMesh);

  ivec2s strsize = cgTextStrSize(font, str, fontSize);

  size_t lines = strsize.y / fontSize;
  int linesWidths[lines];
  memset(linesWidths, 0, sizeof(linesWidths));

  int currentLine = 0;

  for (size_t i = 0; i < strlen(str); i++) {
    char c = str[i];

    if (c == '\n') {
      currentLine++;
      continue;
    }

    int advance, leftSideBearing;
    stbtt_GetCodepointHMetrics(font->info, c, &advance, &leftSideBearing);

    advance *= scale;
    leftSideBearing *= scale;

    int x1, y1, x2, y2;
    stbtt_GetCodepointBitmapBox(font->info, c,
                                scale, scale,
                                &x1, &y1,
                                &x2, &y2);

    linesWidths[currentLine] += advance;
  }

  int charOffsetX;
  int charOffsetY;

  switch (f_cgTextCurrentValign) {
  case CG_TEXT_VALIGN_TOP:
    charOffsetY = 0;
    break;
  case CG_TEXT_VALIGN_MIDDLE:
    charOffsetY = strsize.y / 2;
    break;
  case CG_TEXT_VALIGN_BOTTOM:
    charOffsetY = strsize.y;
    break;
  default:
    charOffsetY = 0;
  }

  currentLine = -1;

  for (size_t i = 0; i < strlen(str); i++) {
    char c = str[i];
    ivec2s charSize = cgTextCharSize(font, c, fontSize);

    if ((c == '\n') || (i == 0)) {
      charOffsetY -= fontSize;
      currentLine++;

      switch (f_cgTextCurrentAlign) {
      case CG_TEXT_ALIGN_LEFT:
        charOffsetX = 0;
        break;
      case CG_TEXT_ALIGN_CENTER:
        charOffsetX = -linesWidths[currentLine] / 2;
        break;
      case CG_TEXT_ALIGN_RIGHT:
        charOffsetX = -linesWidths[currentLine];
        break;
      default:
        charOffsetX = 0;
      }

      if (c == '\n')
        continue;
    }

    unsigned char bitmap[charSize.x * charSize.y];
    memset(bitmap, 0, sizeof(bitmap));

    int advance, leftSideBearing;
    stbtt_GetCodepointHMetrics(font->info, c, &advance, &leftSideBearing);
    advance *= scale;
    leftSideBearing *= scale;

    stbtt_MakeCodepointBitmap(font->info, bitmap,
                              charSize.x, charSize.y,
                              charSize.x,
                              scale, scale, c);

    cg_texture_t tex = cgTextureLoad(bitmap,
                                     GL_UNSIGNED_BYTE,
                                     charSize.x, charSize.y,
                                     GL_RED, GL_RED,
                                     GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,
                                     CGS_TEX_FLTR, CGS_TEX_FLTR);

    int x1, y1, x2, y2;
    stbtt_GetCodepointBitmapBox(font->info, c,
                                scale, scale,
                                &x1, &y1,
                                &x2, &y2);

#ifdef CG_STRANGE_TEXT
    int extraOffsetX = 0;
    int extraOffsetY = 0;
#else // CG_STRANGE_TEXT
    int extraOffsetX = charSize.x / 2;
    int extraOffsetY = charSize.y / 2;
#endif // CG_STRANGE_TEXT

    mat4s transMat = GLMS_MAT4_IDENTITY;
    transMat = glms_translate(transMat, (vec3s) {
        x + charOffsetX + extraOffsetX + leftSideBearing,
        y + charOffsetY + extraOffsetY - y2,
        z,
      });

    float randRot = (float) rand() / (float) (RAND_MAX / 0.25f);
    transMat = glms_rotate(transMat,
                           sin((glfwGetTime() + c + randRot) * 2.0f) * 0.05f,
                           GLMS_ZUP);

    int rndfx = rand();
    int rndfxBound0 = RAND_MAX / 1000;
    int rndfxBound1 = RAND_MAX / 1000 * 2;

    if (rndfx < rndfxBound0)
      transMat = glms_rotate(transMat, 3.14f, GLMS_ZUP);
    if (rndfx >= rndfxBound0 && rndfx < rndfxBound1)
      transMat = glms_scale(transMat, (vec3s) { 0.0f, 0.0f, 0.0f });

    glUniformMatrix4fv(glGetUniformLocation(f_cgTextProg, "u_projMat"),
                       1, false, projection2D.raw[0]);
    glUniformMatrix4fv(glGetUniformLocation(f_cgTextProg, "u_transMat"),
                       1, false, transMat.raw[0]);
    glUniform2f(glGetUniformLocation(f_cgTextProg, "u_size"),
                charSize.x, charSize.y);
    glUniform4fv(glGetUniformLocation(f_cgTextProg, "u_color"),
                 1, f_cgTextCurrentColor.raw);

    cgTextureBind(tex);
    cgMeshDraw(f_cgTextMesh);
    cgTextureUnbind();
    cgTextureDelete(tex);

    charOffsetX += advance;
  }

  cgShaderProgUnbind();
  cgMeshUnbind();
}
