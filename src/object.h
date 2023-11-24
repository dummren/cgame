#ifndef __CG__OBJECT_H__
#define __CG__OBJECT_H__

#include <stddef.h>
#include <stdbool.h>

#include "cglm/struct.h"

#include "shader.h"
#include "material.h"
#include "mesh.h"
#include "camera.h"
#include "physics.h"

#define CG_OBJECT_MAX_MATERIALS 8

typedef struct {
  cg_shader_prog_t prog;
  cg_mesh_t *mesh;
  cg_material_t *materials[CG_OBJECT_MAX_MATERIALS];
  unsigned int *materialSlots;

  vec3s pos;
  vec3s rot;

  cg_physics_collider_t *collider;
} cg_object_t;

cg_object_t *cgObject();
cg_object_t *cgObjectMake(cg_shader_prog_t,
                          cg_mesh_t*,
                          cg_material_t**);

mat4s cgObjectTrans(cg_object_t*);
void cgObjectDraw(cg_object_t*, mat4s, mat4s);

void cgObjectDelete(cg_object_t**);

#endif // __CG__OBJECT_H__
