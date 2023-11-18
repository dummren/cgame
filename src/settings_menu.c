#include "settings_menu.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "glcore.h"
#include "options.h"
#include "settings.h"

bool cgSettingsMenuDraw() {
  static int selected = 0;
  static size_t paramsLen = 4;

  cg_option_arg_t *params[] = {
    cgOptionButton("back"),
    cgOptionSlider("volume", 0, 5, (int) (CGS_GAIN / 0.2f)),
    cgOptionSwitch("texture filtration", CGS_TEX_FLTR == GL_LINEAR),
    cgOptionSwitch("bloom", CGS_BLOOM),
  };

  cgOptionsV(params, paramsLen, &selected);

  CGS_GAIN =
    (float) params[1]->option.sSlider.value
    / (float) params[1]->option.sSlider.max;

  CGS_TEX_FLTR = params[2]->option.sSwitch.value ? GL_LINEAR : GL_NEAREST;
  CGS_BLOOM = params[3]->option.sSwitch.value;

  bool result = params[0]->option.sButton.pressed;

  for (size_t i = 0; i < paramsLen; i++)
    free(params[i]);

  return result;
}
