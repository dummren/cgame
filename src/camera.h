#ifndef __CG__CAMERA_H__
#define __CG__CAMERA_H__

#include "cglm/struct.h"

typedef struct {
  float yFov;
  float zNear;
  float zFar;

  vec3s pos;
  vec3s rot;
} cg_camera_t;

cg_camera_t *cgCamera(const float, const float, const float);

mat4s cgCameraProj(const cg_camera_t*);
mat4s cgCameraView(const cg_camera_t*);

#endif // __CG__CAMERA_H__
