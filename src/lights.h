#ifndef __CG__LIGHTS_H__
#define __CG__LIGHTS_H__

#include <stdbool.h>

#include "cglm/struct.h"

#define CG_LIGHTS_POINTS_MAX 128

typedef struct {
  vec3s pos;
  vec3s col;
  float radius;
} cg_lights_point_t;

extern cg_lights_point_t cgLightsPoints[CG_LIGHTS_POINTS_MAX];

void cgLightsInit();
bool cgLightsIsPointSet(cg_lights_point_t);
void cgLightsPointsClear();
int cgLightsPointAdd(const float, const float, const float,
                     const float, const float, const float,
                     const float);

#endif // __CG__LIGHTS_H__
