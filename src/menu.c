#include "menu.h"

#include <string.h>
#include <stdbool.h>

#include "cglm/struct.h"

#include "main.h"
#include "window.h"
#include "text.h"
#include "settings.h"
#include "shaders.h"
#include "fonts.h"
#include "res.h"
#include "sfx.h"
#include "options.h"
#include "consts.h"
#include "settings_menu.h"

typedef enum {
  f_LOCATION_MAIN,
  f_LOCATION_SETTINGS,
  f_LOCATION_CREDITS,
} f_cg_menu_location_t;

static cg_text_font_t *f_font;
static f_cg_menu_location_t f_location = f_LOCATION_MAIN;

cg_start_func_t cgMenuStart() {
  f_font = cgFontsVideotype;
}

cg_update_func_t cgMenuUpdate() {

}

cg_draw_func_t cgMenuDraw() {
  static const char title[] = "CGAME\0";
  cgTextFontBind(f_font);

  switch(f_location) {

  case f_LOCATION_MAIN:
    {
      cgTextReset();
      cgTextAlign(CG_TEXT_ALIGN_CENTER);
      cgTextValign(CG_TEXT_VALIGN_MIDDLE);
      cgTextColor(0.5f, 0.7f, 0.5f, 1.0f);
      cgTextDraw(WND_WIDTH / 2.0f, WND_HEIGHT / 2.0f, 5.0f,
                 title, FONTSIZE_TITLE);

      static const size_t buttonsCount = 4;
      static const char *buttons[] = {
        "settings\0",
        "play\0",
        "credits\0",
        "exit\0",
      };

      cgTextReset();
      cgTextAlign(CG_TEXT_ALIGN_CENTER);
      cgTextValign(CG_TEXT_VALIGN_MIDDLE);

      static int selected = 1;
      cgOptionsH(buttons, buttonsCount, &selected);

      if (isAcceptB)
        switch (selected) {
        case 0:
          cgSfxAccept();
          f_location = f_LOCATION_SETTINGS;
          break;
        case 1:
          scene = CG_SCENE_GAME;
          break;
        case 3:
          cgSfxAccept();
          cgWindowClose(window);
          break;
        }
    }

    break; // f_LOCATION_MAIN

  case f_LOCATION_SETTINGS:
    {
      cgTextReset();
      cgTextAlign(CG_TEXT_ALIGN_CENTER);
      cgTextValign(CG_TEXT_VALIGN_TOP);
      cgTextColor(0.5f, 0.7f, 0.5f, 1.0f);
      cgTextDraw(WND_WIDTH / 2.0f, WND_HEIGHT, 5.0f,
                 title, FONTSIZE_TITLE_ALT);

      cgTextReset();
      cgTextAlign(CG_TEXT_ALIGN_CENTER);
      cgTextValign(CG_TEXT_VALIGN_BOTTOM);

      if (cgSettingsMenuDraw())
        f_location = f_LOCATION_MAIN;
    }

    break; // f_LOCATION_SETTINGS
  }
}

cg_end_func_t cgMenuEnd() {

}
