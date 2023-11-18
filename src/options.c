#include "options.h"

#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#include "menu.h"
#include "text.h"
#include "consts.h"
#include "sfx.h"

cg_option_arg_t *cgOptionNone(const char *name) {
  cg_option_arg_t *arg = malloc(sizeof(cg_option_arg_t));

  arg->name = name;
  arg->type = CG_OPTION_NONE;
  arg->option = (cg_option_var_t) { {} };

  return arg;
}

cg_option_arg_t *cgOptionButton(const char *name) {
  cg_option_var_t var;
  var.sButton = (cg_option_button_t) { false };

  cg_option_arg_t *arg = malloc(sizeof(cg_option_arg_t));
  arg->name = name;
  arg->type = CG_OPTION_BUTTON;
  arg->option = var;

  return arg;
}

cg_option_arg_t *cgOptionSwitch(const char *name, bool initValue) {
  cg_option_var_t var;
  var.sSwitch = (cg_option_switch_t) { initValue };

  cg_option_arg_t *arg = malloc(sizeof(cg_option_arg_t));
  arg->name = name;
  arg->type = CG_OPTION_SWITCH;
  arg->option = var;

  return arg;
}

cg_option_arg_t *cgOptionSlider(const char *name,
                               int min, int max, int value) {
  cg_option_var_t var;
  var.sSlider = (cg_option_slider_t) { min, max, value };

  cg_option_arg_t *arg = malloc(sizeof(cg_option_arg_t));
  arg->name = name;
  arg->type = CG_OPTION_SLIDER;
  arg->option = var;

  return arg;
}

cg_option_arg_t *cgOptionList(const char *name,
                             int len, int sel, char **strings) {
  cg_option_var_t var;
  var.sList = (cg_option_list_t) { len, sel, strings };

  cg_option_arg_t *arg = malloc(sizeof(cg_option_arg_t));
  arg->name = name;
  arg->type = CG_OPTION_LIST;
  arg->option = var;

  return arg;
}

void cgOptionsH(const char **buttons,
                const size_t count,
                int *selected) {
  cg_text_font_t *font = cgTextFontCurrent();

  if (isLeftPressedB) {
    (*selected)--;
    cgSfxBeep();
  }

  if (isRightPressedB) {
    (*selected)++;
    cgSfxBeep();
  }

  if (*selected < 0)
    *selected = count - 1;
  if (*selected >= count)
    *selected = 0;

  const float start = WND_WIDTH / count * 0.5f;
  const float step = WND_WIDTH / count;

  for (size_t i = 0; i < count; i++) {
    const float posX = start + step * i;

    const float leftArrowPosX = posX
      - cgTextStrSize(font, buttons[i], FONTSIZE_TEXT).x / 2
      - cgTextStrSize(font, "<", FONTSIZE_TEXT).x / 2;

    const float rightArrowPosX = posX
      + cgTextStrSize(font, buttons[i], FONTSIZE_TEXT).x / 2
      + cgTextStrSize(font, ">", FONTSIZE_TEXT).x / 2;

    vec4s color = *selected == i ? TEXT_SEL_CLR : TEXT_CLR;
    cgTextColor(color.r, color.g, color.b, color.a);

    if (i == *selected) {
      cgTextDraw(leftArrowPosX, WND_HEIGHT / 4.0f, 5.0f, "<", FONTSIZE_TEXT);
      cgTextDraw(rightArrowPosX, WND_HEIGHT / 4.0f, 5.0f, ">", FONTSIZE_TEXT);
    }

    cgTextDraw(posX, WND_HEIGHT / 4.0f, 5.0f, buttons[i], FONTSIZE_TEXT);
  }
}

void cgOptionsV(cg_option_arg_t **params,
                const size_t count,
                int *selected) {
  cg_text_font_t *font = cgTextFontCurrent();

  if (isDownPressedB) {
    (*selected)--;
    cgSfxBeep();
  }

  if (isUpPressedB) {
    (*selected)++;
    cgSfxBeep();
  }

  if (*selected < 0)
    *selected = count - 1;
  if (*selected >= count)
    *selected = 0;

  const float start = FONTSIZE_TEXT;
  const float step = FONTSIZE_TEXT;

  for (size_t i = 0; i < count; i++) {
    char *text = malloc(sizeof(char) * 128);
    strcpy(text, params[i]->name);

    switch (params[i]->type) {

    case CG_OPTION_BUTTON:
      {
        cg_option_button_t *paramsStruct = &params[i]->option.sButton;
        bool *pressed = &paramsStruct->pressed;

        if (isAcceptB && *selected == i) {
          *pressed = true;
          cgSfxAccept();
        }
      }

      break; // CG_OPTION_BUTTON

    case CG_OPTION_SWITCH:
      {
        cg_option_switch_t *paramsStruct = &params[i]->option.sSwitch;
        bool *value = &paramsStruct->value;

        if ((isLeftPressedB || isRightPressedB || isAcceptB)
            && *selected == i) {
          *value = !*value;
          cgSfxBeep();
        }

        strcat(text, *value ? ": YES" : ": NO");
      }

      break; // CG_OPTION_SWITCH

    case CG_OPTION_SLIDER:
      {
        cg_option_slider_t *sliderStruct = &params[i]->option.sSlider;

        int sliderMin = sliderStruct->min;
        int sliderMax = sliderStruct->max;
        int *sliderValue = &sliderStruct->value;

        if (isLeftPressedB && *selected == i) {
          (*sliderValue)--;
          cgSfxBeep();
        }

        if (isRightPressedB && *selected == i) {
          (*sliderValue)++;
          cgSfxBeep();
        }

        if (*sliderValue < sliderMin)
          *sliderValue = sliderMin;
        if (*sliderValue > sliderMax)
          *sliderValue = sliderMax;

        strcat(text, ": [");

        for (int i = sliderMin; i < *sliderValue; i++)
          strcat(text, "=");

        for (int i = *sliderValue; i < sliderMax; i++)
          strcat(text, "-");

        strcat(text, "]");
      }

      break; // CG_OPTION_SLIDER

    case CG_OPTION_LIST:
      {
        cg_option_list_t *listStruct = &params[i]->option.sList;
        int listLen = listStruct->len;
        int *listSel = &listStruct->sel;
        char **list = listStruct->strings;

        if ((isAcceptB || isRightPressedB) && *selected == i) {
          (*listSel)++;
          cgSfxBeep();
        }

        if (isLeftPressedB && *selected == i) {
          (*listSel)--;
          cgSfxBeep();
        }

        if (*listSel < 0)
          *listSel = listLen - 1;
        if (*listSel >= listLen)
          *listSel = 0;

        sprintf(text, "%s: %d. %s", text, *listSel + 1, list[*listSel]);
      }

      break; // CG_OPTION_LIST

    };

    const float posY = start + step * i;

    const float leftArrowPosX = WND_WIDTH / 2
      - cgTextStrSize(font, text, FONTSIZE_TEXT).x / 2
      - cgTextStrSize(font, "<", FONTSIZE_TEXT).x / 2;

    const float rightArrowPosX = WND_WIDTH / 2
      + cgTextStrSize(font, text, FONTSIZE_TEXT).x / 2
      + cgTextStrSize(font, ">", FONTSIZE_TEXT).x / 2;

    vec4s color = *selected == i ? TEXT_SEL_CLR : TEXT_CLR;
    cgTextColor(color.r, color.g, color.b, color.a);

    if (i == *selected) {
      cgTextDraw(leftArrowPosX, posY, 5.0f, "<", FONTSIZE_TEXT);
      cgTextDraw(rightArrowPosX, posY, 5.0f, ">", FONTSIZE_TEXT);
    }

    cgTextDraw(WND_WIDTH / 2, posY, 5.0f, text, FONTSIZE_TEXT);
    free(text);
  }
}
