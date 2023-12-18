#include "lights.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "cglm/struct.h"

cg_lights_point_t cgLightsPoints[CG_LIGHTS_POINTS_MAX];

// Predefine a default light to avoid re-creation in cgLightsIsPointSet
static const cg_lights_point_t defaultPoint = {
  .pos = {0.0f, 0.0f, 0.0f},
  .col = {0.0f, 0.0f, 0.0f},
  .radius = 0.0f,
};

void cgLightsInit() {
  memset(cgLightsPoints, 0, sizeof(cgLightsPoints));
}

bool cgLightsIsPointSet(const cg_lights_point_t *point) {
  // Use memcmp to compare with the defaultPoint
  return memcmp(&point->col, &defaultPoint.col, sizeof(vec3s)) != 0;
}

void cgLightsPointsClear() {
  // Use memset for better performance instead of looping
  memset(cgLightsPoints, 0, sizeof(cgLightsPoints));
}

void cgLightsPoint(const float x, const float y, const float z,
                   const float r, const float g, const float b,
                   const float radius) {
  for (size_t i = 0; i < CG_LIGHTS_POINTS_MAX; i++) {
    if (!cgLightsIsPointSet(&cgLightsPoints[i])) {
      // Directly assign values to the array element
      cgLightsPoints[i].pos = (vec3s) { x, y, z };
      cgLightsPoints[i].col = (vec3s) { r, g, b };
      cgLightsPoints[i].radius = radius;
      break;
    }
  }
}
