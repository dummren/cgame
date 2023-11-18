#include "material.h"

#include <stdbool.h>

#include "cglm/struct.h"

#include "texture.h"

cg_material_t *cgMaterial() {
  cg_material_t *material = malloc(sizeof(cg_material_t));

  material->albedoCol = (vec4s) { 1.0f, 1.0f, 1.0f, 1.0f };
  material->brightCol = (vec3s) { 0.0f, 0.0f, 0.0f };

  material->albedoTexEnabled = false;
  material->brightTexEnabled = false;

  material->albedoTex = 0;
  material->brightTex = 0;

  return material;
}

cg_material_t *cgMaterialMake(vec4s albedoCol, vec3s brightCol,
                              bool albedoTexEnabled, bool brightTexEnabled,
                              cg_texture_t albedoTex, cg_texture_t brightTex) {
  cg_material_t *material = cgMaterial();

  material->albedoCol = albedoCol;
  material->brightCol = brightCol;

  material->albedoTexEnabled = albedoTexEnabled;
  material->brightTexEnabled = brightTexEnabled;

  material->albedoTex = albedoTex;
  material->brightTex = brightTex;

  return material;
}
