#include "lights.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "cglm/struct.h"

cg_lights_point_t cgLightsPoints[CG_LIGHTS_POINTS_MAX];

void cgLightsInit() {
  cgLightsPointsClear();
}

bool cgLightsIsPointSet(cg_lights_point_t point) {
  cg_lights_point_t defaultPoint = {
    (vec3s) { 0.0f, 0.0f, 0.0f },
    (vec3s) { 0.0f, 0.0f, 0.0f },
    0.0f,
  };

  return memcmp(point.col.raw, defaultPoint.col.raw, sizeof(vec3s)) == 0;
}

void cgLightsPointsClear() {
  for (size_t i = 0; i < CG_LIGHTS_POINTS_MAX; i++)
    cgLightsPoints[i] = (cg_lights_point_t) {
      (vec3s) { 0.0f, 0.0f, 0.0f },
      (vec3s) { 0.0f, 0.0f, 0.0f },
      0.0f,
    };
}

int cgLightsPointAdd(const float x, const float y, const float z,
                     const float r, const float g, const float b,
                     const float radius) {
  for (size_t i = 0; i < CG_LIGHTS_POINTS_MAX; i++)
    if (cgLightsIsPointSet(cgLightsPoints[i])) {
      cg_lights_point_t light;

      light.pos = (vec3s) { x, y, z };
      light.col = (vec3s) { r, g, b };
      light.radius = radius;

      cgLightsPoints[i] = light;

      return i;
    }
}
