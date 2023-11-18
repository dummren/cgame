#include "camera.h"

#include <stdlib.h>

#include "cglm/struct.h"

#include "main.h"

cg_camera_t *cgCamera(const float yFov, const float zNear, const float zFar) {
  cg_camera_t *camera = malloc(sizeof(cg_camera_t));

  camera->yFov = yFov;
  camera->zNear = zNear;
  camera->zFar = zFar;

  camera->pos = GLMS_VEC3_ZERO;
  camera->rot = GLMS_VEC3_ZERO;

  return camera;
}

mat4s cgCameraProj(const cg_camera_t *camera) {
  return glms_perspective(camera->yFov,
                          (float) WND_WIDTH / (float) WND_HEIGHT,
                          camera->zNear, camera->zFar);
}

mat4s cgCameraView(const cg_camera_t *camera) {
  mat4s view = GLMS_MAT4_IDENTITY;

  view = glms_translate(view, glms_vec3_negate(camera->pos));
  view = glms_rotate(view, -camera->rot.x, GLMS_XUP);
  view = glms_rotate(view, -camera->rot.y, GLMS_YUP);
  view = glms_rotate(view, -camera->rot.z, GLMS_ZUP);

  return view;
}
