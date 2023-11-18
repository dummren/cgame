#include "fonts.h"

#include "text.h"
#include "res.h"

cg_text_font_t *cgFontsVideotype;

static void f_load(cg_text_font_t **font, const char *path) {
  // stb_truetype needs this data for every call so it has to be
  // manually allocated
  unsigned char *data = CG_RES_MALLOC(path);
  cgResRead(path, data);
  *font = cgTextFont(data);
}

void cgFontsInit() {
  f_load(&cgFontsVideotype, "fnt/videotype.otf");
}
