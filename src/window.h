#ifndef __CG__WINDOW_H__
#define __CG__WINDOW_H__

#include <stdbool.h>

#include "cglm/struct.h"

#include "glcore.h"

typedef GLFWwindow* cg_window_t;
typedef GLFWmonitor* cg_monitor_t;

cg_window_t cgWindow(const int, const int, const char*, const cg_monitor_t);

void cgWindowMakeCurrent(const cg_window_t);
void cgWindowSwapBuffers(const cg_window_t);

bool cgWindowIsOpen(const cg_window_t);
void cgWindowClose(const cg_window_t);

int cgWindowKeyState(const cg_window_t, const int);
bool cgWindowIsKeyPressed(const cg_window_t, const int);

// bios-like input
int cgWindowKeyStateB(const cg_window_t, const int);
bool cgWindowIsKeyPressedB(const cg_window_t, const int);

vec2s cgWindowRelCursorPos(const cg_window_t);

void cgWindowResetPressedKeysB(const cg_window_t);
void cgWindowResetRelCursorPos(const cg_window_t);

#endif // __CG__WINDOW_H__
