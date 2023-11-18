#ifndef __CG__OPTIONS_H__
#define __CG__OPTIONS_H__

#include <stddef.h>
#include <stdbool.h>

typedef enum {
  CG_OPTION_NONE,
  CG_OPTION_BUTTON,
  CG_OPTION_SWITCH,
  CG_OPTION_SLIDER,
  CG_OPTION_LIST,
} cg_option_t;

typedef struct {} cg_option_none_t;

typedef struct {
  bool pressed;
} cg_option_button_t;

typedef struct {
  bool value;
} cg_option_switch_t;

typedef struct {
  int min, max, value;
} cg_option_slider_t;

typedef struct {
  int len, sel;
  char **strings;
} cg_option_list_t;

typedef union {
  cg_option_none_t sNone;
  cg_option_button_t sButton;
  cg_option_switch_t sSwitch;
  cg_option_slider_t sSlider;
  cg_option_list_t sList;
} cg_option_var_t;

typedef struct {
  const char *name;
  cg_option_t type;
  cg_option_var_t option;
} cg_option_arg_t;

cg_option_arg_t *cgOptionNone(const char*);
cg_option_arg_t *cgOptionButton(const char*);
cg_option_arg_t *cgOptionSwitch(const char*, bool);
cg_option_arg_t *cgOptionSlider(const char*, int, int, int);
cg_option_arg_t *cgOptionList(const char*, int, int, char**);

void cgOptionsH(const char**, const size_t, int*);
void cgOptionsV(cg_option_arg_t**,
                const size_t,
                int*);

#endif // __CG__OPTIONS_H__
