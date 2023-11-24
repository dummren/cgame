#ifndef __CG__PHYSICS_H__
#define __CG__PHYSICS_H__

#include <stdbool.h>
#include <stddef.h>

#include "cglm/struct.h"

#define CG_PHYSICS_WORLD_MAX_OBJECTS 512

typedef struct {
  bool enabled;
  bool dynamic;
  vec3s size;
  vec3s pos;
  float stepHeight;
} cg_physics_collider_t;

typedef struct {
  cg_physics_collider_t *colliders[CG_PHYSICS_WORLD_MAX_OBJECTS];
} cg_physics_world_t;

cg_physics_world_t *cgPhysicsWorld();
cg_physics_collider_t *cgPhysicsCollider(bool, bool, vec3s, vec3s);

// no limit checks >;)
void cgPhysicsWorldAdd(cg_physics_world_t**, cg_physics_collider_t*);
vec3s cgPhysicsWorldAttemptMove(cg_physics_world_t*,
                                cg_physics_collider_t*,
                                vec3s,
                                bool*);

void cgPhysicsWorldDelete(cg_physics_world_t**);

#endif // __CG__PHYSICS_H__
