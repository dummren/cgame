#include "main.h"

#include <string.h>
#include <stdbool.h>

#include "cglm/struct.h"

#include "glcore.h"
#include "text.h"
#include "core.h"
#include "window.h"
#include "menu.h"
#include "game.h"
#include "texture.h"
#include "shader.h"
#include "mesh.h"
#include "settings.h"
#include "audio.h"
#include "shaders.h"
#include "textures.h"
#include "res.h"
#include "fonts.h"
#include "sfx.h"
#include "consts.h"
#include "lights.h"
#include "prefabs.h"

const cg_start_func_t (*SCENES_START_FUNCS[SCENES_COUNT])() = {
  cgMenuStart,
  cgGameStart,
};

const cg_update_func_t (*SCENES_UPDATE_FUNCS[SCENES_COUNT])() = {
  cgMenuUpdate,
  cgGameUpdate,
};

const cg_draw_func_t (*SCENES_DRAW_FUNCS[SCENES_COUNT])() = {
  cgMenuDraw,
  cgGameDraw,
};

const cg_end_func_t (*SCENES_END_FUNCS[SCENES_COUNT])() = {
  cgMenuEnd,
  cgGameEnd,
};

const unsigned int CANVAS_INDICES[3 * 2] = {
  0, 1, 2,
  2, 3, 0,
};

const float CANVAS_POSITIONS[3 * 4] = {
  1.0f, 1.0f, 0.0f,
  -1.0f, 1.0f, 0.0f,
  -1.0f, -1.0f, 0.0f,
  1.0f, -1.0f, 0.0f,
};

const float CANVAS_UV[2 * 4] = {
  1.0f, 1.0f,
  0.0f, 1.0f,
  0.0f, 0.0f,
  1.0f, 0.0f,
};

cg_window_t window = NULL;

vec4s clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };

static cg_scene_t f_prevScene = CG_SCENE_MENU;
cg_scene_t scene = CG_SCENE_MENU;

mat4s projection2D;

int upStateB, downStateB, leftStateB, rightStateB,
  enterStateB, spaceStateB, kpEnterStateB;

bool isUpPressedB, isDownPressedB, isLeftPressedB, isRightPressedB,
  isEnterPressedB, isSpacePressedB, isKpEnterPressedB;

bool isAcceptB;

void updateKeysB() {
  upStateB = cgWindowKeyStateB(window, GLFW_KEY_UP);
  downStateB = cgWindowKeyStateB(window, GLFW_KEY_DOWN);
  leftStateB = cgWindowKeyStateB(window, GLFW_KEY_LEFT);
  rightStateB = cgWindowKeyStateB(window, GLFW_KEY_RIGHT);
  enterStateB = cgWindowKeyStateB(window, GLFW_KEY_ENTER);
  spaceStateB = cgWindowKeyStateB(window, GLFW_KEY_SPACE);
  kpEnterStateB = cgWindowKeyStateB(window, GLFW_KEY_KP_ENTER);

#define IS_PRESSED(state) ((state == GLFW_PRESS) || (state == GLFW_REPEAT))
  isUpPressedB = IS_PRESSED(upStateB);
  isDownPressedB = IS_PRESSED(downStateB);
  isLeftPressedB = IS_PRESSED(leftStateB);
  isRightPressedB = IS_PRESSED(rightStateB);
  isEnterPressedB = IS_PRESSED(enterStateB);
  isSpacePressedB = IS_PRESSED(spaceStateB);
  isKpEnterPressedB = IS_PRESSED(kpEnterStateB);
#undef IS_PRESSED

  isAcceptB = isEnterPressedB || isSpacePressedB || isKpEnterPressedB;
}

int main(int argc, char *argv[]) {
  projection2D = glms_ortho(0.0f, WND_WIDTH,
                            0.0f, WND_HEIGHT,
                            -100.0f, 100.0f);
  cgCoreInit();

  glfwWindowHint(GLFW_RESIZABLE, false);
  const GLFWvidmode *videomode = glfwGetVideoMode(WND_MONITOR);
  window = cgWindow(videomode->width, videomode->height,
                    WND_TITLE, WND_MONITOR);
  cgWindowMakeCurrent(window);

  if (glfwRawMouseMotionSupported())
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, true);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  cgResInit();
  cgAudioInit();
  cgShadersInit();
  cgTexturesInit();
  cgTextInit();
  cgFontsInit();
  cgSfxInit();
  cgLightsInit();

  cg_shader_t prog = cgShadersCanvasProg;
  cg_mesh_t *mesh = cgMesh();
  cgMeshStoreIndices(mesh, CANVAS_INDICES, sizeof(CANVAS_INDICES), 0);
  cgMeshStoref(mesh, CANVAS_POSITIONS, sizeof(CANVAS_POSITIONS), 1, 3);
  cgMeshStoref(mesh, CANVAS_UV, sizeof(CANVAS_UV), 2, 2);

  bool isLoading = true;

  while (cgWindowIsOpen(window)) {
    glfwPollEvents();
    updateKeysB();

    if (glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED)
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    unsigned int renderbuffer;
    glGenRenderbuffers(1, &renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);

    unsigned int fbTexFmt =
      rand() > RAND_MAX / 512 ? TEX_FMT : TEX_FMT_DISTORT;

    cg_texture_t fbFragTex =
      cgTextureEmpty(GL_FLOAT,
                     WND_WIDTH, WND_HEIGHT,
                     fbTexFmt, GL_RGBA,
                     GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,
                     CGS_TEX_FLTR, CGS_TEX_FLTR);

    cg_texture_t fbBrightTex =
      cgTextureEmpty(GL_FLOAT,
                     WND_WIDTH, WND_HEIGHT,
                     fbTexFmt, GL_RGBA,
                     GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,
                     CGS_TEX_FLTR, CGS_TEX_FLTR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 0,
                           GL_TEXTURE_2D, fbFragTex, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 1,
                           GL_TEXTURE_2D, fbBrightTex, 0);

    glRenderbufferStorage(GL_RENDERBUFFER, CG_DEPTH_COMPONENT,
                          WND_WIDTH, WND_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, renderbuffer);

    const unsigned int buffers[] = {
      GL_COLOR_ATTACHMENT0 + 0,
      GL_COLOR_ATTACHMENT0 + 1,
    };

    glDrawBuffers(2, buffers);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, WND_WIDTH, WND_HEIGHT);

    if (!isLoading && f_prevScene != scene) {
      SCENES_END_FUNCS[f_prevScene]();
      SCENES_START_FUNCS[scene]();

      f_prevScene = scene;
    }

    if (isLoading) {
      cgTextReset();
      cgTextFontBind(cgFontsVideotype);
      cgTextAlign(CG_TEXT_ALIGN_CENTER);
      cgTextValign(CG_TEXT_VALIGN_MIDDLE);
      cgTextColor(1.0f, 1.0f, 1.0f, 1.0f);
      cgTextDraw(WND_WIDTH / 2, WND_HEIGHT / 2, 5.0f,
        "Very non optimized models loading...\nplease wait", 25.0f);

      SCENES_START_FUNCS[scene]();
    }

    if (!isLoading) {
      SCENES_UPDATE_FUNCS[scene]();
      SCENES_DRAW_FUNCS[scene]();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, videomode->width, videomode->height);

    { // preparing stuff for rendering
      cgShaderProgBind(prog);

      glActiveTexture(GL_TEXTURE0 + 0);
      cgTextureBind(fbFragTex);

      glActiveTexture(GL_TEXTURE0 + 1);
      cgTextureBind(fbBrightTex);

      glUniform1f(glGetUniformLocation(prog, "u_time"), glfwGetTime());

      glUniform1i(glGetUniformLocation(prog, "u_bloomEnabled\0"), CGS_BLOOM);

      glUniform1i(glGetUniformLocation(prog, "u_fragTex\0"), 0);
      glUniform1i(glGetUniformLocation(prog, "u_brightTex\0"), 1);
    } // preparing stuff for rendering

    cgMeshDraw(mesh);

    { // unbinding
      cgShaderProgUnbind();

      glActiveTexture(GL_TEXTURE0 + 0);
      cgTextureUnbind();

      glActiveTexture(GL_TEXTURE0 + 1);
      cgTextureUnbind();

      glActiveTexture(GL_TEXTURE0 + 0);
    } // unbinding

    glDeleteFramebuffers(1, &framebuffer);
    glDeleteRenderbuffers(1, &renderbuffer);
    cgTextureDelete(fbFragTex);
    cgTextureDelete(fbBrightTex);

    cgWindowSwapBuffers(window);
    cgWindowResetPressedKeysB(window);
    cgWindowResetRelCursorPos(window);

    if (isLoading) {
      cgPrefabsInit();
      isLoading = false;
      cgTextReset();
    }
  }

  SCENES_END_FUNCS[scene]();

  cgCoreTerminate();
  return 0;
}
