#ifndef __CG__MAIN_H__
#define __CG__MAIN_H__

#include "cglm/struct.h"

#include "glcore.h"
#include "window.h"

#define WND_WIDTH 500
#define WND_HEIGHT 300
#define WND_TITLE "cgame\0"
#define WND_MONITOR glfwGetPrimaryMonitor()

#define SCENES_COUNT 2

typedef void cg_start_func_t;
typedef void cg_update_func_t;
typedef void cg_draw_func_t;
typedef void cg_end_func_t;

typedef enum {
  CG_SCENE_MENU,
  CG_SCENE_GAME,
} cg_scene_t;

extern const cg_start_func_t (*SCENES_START_FUNCS[SCENES_COUNT])();
extern const cg_update_func_t (*SCENES_UPDATE_FUNCS[SCENES_COUNT])();
extern const cg_draw_func_t (*SCENES_DRAW_FUNCS[SCENES_COUNT])();
extern const cg_end_func_t (*SCENES_END_FUNCS[SCENES_COUNT])();

extern cg_window_t window;
extern vec4s clearColor;
extern cg_scene_t scene;
extern mat4s projection2D;

extern int upStateB, downStateB, leftStateB, rightStateB,
  enterStateB, spaceStateB, kpEnterStateB;

extern bool isUpPressedB, isDownPressedB, isLeftPressedB, isRightPressedB,
  isEnterPressedB, isSpacePressedB, isKpEnterPressedB;

extern bool isAcceptB;

void updateKeysB();

#endif // __CG__MAIN_H__
