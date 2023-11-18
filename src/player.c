#include "player.h"

#include <stdlib.h>

#include "cglm/struct.h"

#include "glcore.h"
#include "camera.h"
#include "window.h"
#include "main.h"

cg_player_t *cgPlayer() {
  cg_player_t *player = malloc(sizeof(cg_player_t));

  player->cam = cgCamera(PLAYER_CAM_YFOV, PLAYER_CAM_ZNEAR, PLAYER_CAM_ZFAR);
  player->cam->pos = (vec3s) { 0.0f, PLAYER_HEIGHT, 0.0f };

  player->camRot = GLMS_VEC3_ZERO;
  player->pos = GLMS_VEC3_ZERO;
  player->rot = GLMS_VEC3_ZERO;
  player->velocity = GLMS_VEC3_ZERO;

  return player;
}

mat4s cgPlayerView(const cg_player_t *player) {
  mat4s view = GLMS_MAT4_IDENTITY;

  view = glms_translate(view, glms_vec3_negate(player->pos));

  mat4s playerRot = GLMS_MAT4_IDENTITY;
  playerRot = glms_rotate(playerRot, -player->rot.x, GLMS_XUP);
  playerRot = glms_rotate(playerRot, -player->rot.y, GLMS_YUP);
  playerRot = glms_rotate(playerRot, -player->rot.z, GLMS_ZUP);
  view = glms_mul(playerRot, view);

  view = glms_translate(view, glms_vec3_negate(player->cam->pos));

  mat4s camRot = GLMS_MAT4_IDENTITY;
  camRot = glms_rotate(camRot, -player->cam->rot.x, GLMS_XUP);
  camRot = glms_rotate(camRot, -player->cam->rot.y, GLMS_YUP);
  camRot = glms_rotate(camRot, -player->cam->rot.z, GLMS_ZUP);
  view = glms_mul(camRot, view);

  return view;
}

void cgPlayerUpdate(cg_player_t *player) {
  player->camRot.x -= cgWindowRelCursorPos(window).y * PLAYER_LOOK_SENS;
  player->camRot.y -= cgWindowRelCursorPos(window).x * PLAYER_LOOK_SENS;

  player->camRot.x = glm_clamp(player->camRot.x, -1.57f, 1.57f);

  player->cam->rot.x = glm_lerp(player->cam->rot.x, player->camRot.x,
                                PLAYER_LOOK_SMOOTH);
  player->rot.y = glm_lerp(player->rot.y, player->camRot.y,
                           PLAYER_LOOK_SMOOTH);

  vec3s move = { 0.0f, 0.0f, 0.0f };

  if (cgWindowIsKeyPressed(window, GLFW_KEY_A))
    move.x -= 1.0f;
  if (cgWindowIsKeyPressed(window, GLFW_KEY_D))
    move.x += 1.0f;
  if (cgWindowIsKeyPressed(window, GLFW_KEY_W))
    move.z -= 1.0f;
  if (cgWindowIsKeyPressed(window, GLFW_KEY_S))
    move.z += 1.0f;

  move = glms_vec3_rotate(move, player->rot.y, GLMS_YUP);
  move = glms_vec3_normalize(move);
  move = glms_vec3_mul(move, (vec3s) {
      PLAYER_SPEED,
      PLAYER_SPEED,
      PLAYER_SPEED,
    });

  player->velocity.x = glm_lerp(player->velocity.x, move.x, PLAYER_ACCEL);
  player->velocity.z = glm_lerp(player->velocity.z, move.z, PLAYER_ACCEL);

  player->pos = glms_vec3_add(player->pos, player->velocity);
}
