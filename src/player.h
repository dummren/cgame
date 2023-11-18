#ifndef __CG__PLAYER_H__
#define __CG__PLAYER_H__

#include "cglm/struct.h"

#include "camera.h"

#define PLAYER_HEIGHT 1.8f
#define PLAYER_CAM_YFOV (3.14f / 180.0f * 65.0f)
#define PLAYER_CAM_ZNEAR 0.01f
#define PLAYER_CAM_ZFAR 100.0f
#define PLAYER_LOOK_SENS 0.0175f
#define PLAYER_LOOK_SMOOTH 0.5f
#define PLAYER_SPEED 0.025f
#define PLAYER_ACCEL 0.1f

typedef struct {
  cg_camera_t *cam;
  vec3s camRot;
  vec3s pos;
  vec3s rot;
  vec3s velocity;
} cg_player_t;

cg_player_t *cgPlayer();
mat4s cgPlayerView(const cg_player_t*);

void cgPlayerUpdate(cg_player_t*);

#endif // __CG__PLAYER_H__
