#include "window.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "cglm/struct.h"

#include "glcore.h"

static size_t f_cgWindowsLen = 0;
static cg_window_t *f_cgWindows;

static size_t f_cgWindowKeysStatesLen = 0;
static int **f_cgWindowKeysStates;

static size_t f_cgWindowKeysStatesBLen = 0;
static int **f_cgWindowKeysStatesB;

static size_t f_relPosLen = 0;
static vec2s *f_relPos;

static int f_cgWindowIndex(cg_window_t window) {
  for (size_t i = 0; i < f_cgWindowsLen; i++)
    if (f_cgWindows[i] == window)
      return i;

  return -1;
}

static void f_cgWindowKeyCallback(GLFWwindow *window, int key, int scancode,
                           int action, int mods) {
  if (action == GLFW_PRESS || action == GLFW_RELEASE)
    f_cgWindowKeysStates[f_cgWindowIndex(window)][key] = action;

  f_cgWindowKeysStatesB[f_cgWindowIndex(window)][key] = action;
}

static void f_cursorPosCallback(GLFWwindow *window, double x, double y) {
  static bool isFirstTime = true;

  static double prevX = 0.0;
  static double prevY = 0.0;

  if (isFirstTime) {
    prevX = x;
    prevY = y;
    isFirstTime = false;
  }

  f_relPos[f_cgWindowIndex(window)] = (vec2s) { x - prevX, y - prevY };

  prevX = x;
  prevY = y;
}

cg_window_t cgWindow(const int width,
                     const int height,
                     const char *title,
                     const cg_monitor_t monitor) {
  const GLFWvidmode *videomode = glfwGetVideoMode(monitor);

  glfwWindowHint(GLFW_RED_BITS, videomode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, videomode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, videomode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, videomode->refreshRate);

  cg_window_t window = glfwCreateWindow(width, height, title, monitor, NULL);
  assert(window);

  if (f_cgWindows == NULL)
    f_cgWindows = malloc(0);

  f_cgWindows = realloc(f_cgWindows, ++f_cgWindowsLen * sizeof(cg_window_t));
  f_cgWindows[f_cgWindowsLen - 1] = window;

  f_cgWindowKeysStates = realloc(f_cgWindowKeysStates,
                                 ++f_cgWindowKeysStatesLen * sizeof(int));
  f_cgWindowKeysStatesB = realloc(f_cgWindowKeysStatesB,
                                  ++f_cgWindowKeysStatesBLen * sizeof(int));
  f_relPos = realloc(f_relPos, ++f_relPosLen * sizeof(vec2s));

  f_cgWindowKeysStates[f_cgWindowKeysStatesLen - 1] =
    malloc(GLFW_KEY_LAST * sizeof(int));
  f_cgWindowKeysStatesB[f_cgWindowKeysStatesBLen - 1] =
    malloc(GLFW_KEY_LAST * sizeof(int));
  f_relPos[f_relPosLen - 1] = (vec2s) { 0.0f, 0.0f };

  memset(f_cgWindowKeysStates[f_cgWindowKeysStatesLen - 1],
         GLFW_RELEASE, GLFW_KEY_LAST * sizeof(int));
  memset(f_cgWindowKeysStatesB[f_cgWindowKeysStatesBLen - 1],
         GLFW_RELEASE, GLFW_KEY_LAST * sizeof(int));

  glfwSetKeyCallback(window, f_cgWindowKeyCallback);
  glfwSetCursorPosCallback(window, f_cursorPosCallback);

  return window;
}

void cgWindowMakeCurrent(const cg_window_t window) {
  glfwMakeContextCurrent(window);
  initGlFunctions();
  glfwSwapInterval(1);
}

void cgWindowSwapBuffers(const cg_window_t window) {
  glfwSwapBuffers(window);
}

bool cgWindowIsOpen(const cg_window_t window) {
  return !glfwWindowShouldClose(window);
}

void cgWindowClose(const cg_window_t window) {
  glfwSetWindowShouldClose(window, true);
}

int cgWindowKeyState(const cg_window_t window, const int key) {
  return f_cgWindowKeysStates[f_cgWindowIndex(window)][key];
}

bool cgWindowIsKeyPressed(const cg_window_t window, const int key) {
  return cgWindowKeyState(window, key) == GLFW_PRESS;
}

int cgWindowKeyStateB(const cg_window_t window, const int key) {
  return f_cgWindowKeysStatesB[f_cgWindowIndex(window)][key];
}

bool cgWindowIsKeyPressedB(const cg_window_t window, const int key) {
  return cgWindowKeyStateB(window, key) == GLFW_PRESS;
}

vec2s cgWindowRelCursorPos(const cg_window_t window) {
  return f_relPos[f_cgWindowIndex(window)];
}


void cgWindowResetPressedKeysB(const cg_window_t window) {
  int *states = f_cgWindowKeysStatesB[f_cgWindowIndex(window)];

  for (size_t i = 0; i < GLFW_KEY_LAST; i++)
    if (states[i] == GLFW_PRESS)
      states[i] = GLFW_RELEASE;
}

void cgWindowResetRelCursorPos(const cg_window_t window) {
  f_relPos[f_cgWindowIndex(window)] = (vec2s) { 0.0f, 0.0f };
}
