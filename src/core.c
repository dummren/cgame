#include "core.h"

#include <assert.h>
#include <stdbool.h>

#include "glcore.h"

void cgCoreInit() {
  assert(glfwInit());

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void cgCoreTerminate() {
  glfwTerminate();
}
