#include "shaders.h"

#include <string.h>
#include <stdlib.h>

#include "shader.h"
#include "res.h"

cg_shader_prog_t cgShadersCanvasProg;
cg_shader_prog_t cgShadersTextProg;
cg_shader_prog_t cgShadersObjectProg;

static void f_load(cg_shader_prog_t *prog, const char *vs, const char *fs) {
  unsigned char vssrc[cgResLen(vs)];
  unsigned char fssrc[cgResLen(fs)];

  cgResRead(vs, vssrc);
  cgResRead(fs, fssrc);

  *prog = cgShaderProg(vssrc, fssrc);
}

void cgShadersInit() {
  f_load(&cgShadersCanvasProg, "shdr/canvas.vert", "shdr/canvas.frag");
  f_load(&cgShadersTextProg, "shdr/text.vert", "shdr/text.frag");
  f_load(&cgShadersObjectProg, "shdr/object.vert", "shdr/object.frag");
}
