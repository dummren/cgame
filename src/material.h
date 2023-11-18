#ifndef __CG__MATERIAL_H__
#define __CG__MATERIAL_H__

#include <stdbool.h>

#include "cglm/struct.h"

#include "texture.h"

typedef struct {
  vec4s albedoCol;
  vec3s brightCol;

  bool albedoTexEnabled;
  bool brightTexEnabled;

  cg_texture_t albedoTex;
  cg_texture_t brightTex;
} cg_material_t;

cg_material_t *cgMaterial();
cg_material_t *cgMaterialMake(vec4s, vec3s,
                              bool, bool,
                              cg_texture_t, cg_texture_t);

#endif // __CG__MATERIAL_H__
