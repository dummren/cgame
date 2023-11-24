#include "physics.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "cglm/struct.h"

cg_physics_world_t *cgPhysicsWorld() {
  cg_physics_world_t *physicsWorld = malloc(sizeof(cg_physics_world_t));

  for (size_t i = 0; i < CG_PHYSICS_WORLD_MAX_OBJECTS; i++)
    physicsWorld->colliders[i] = NULL;

  return physicsWorld;
}

cg_physics_collider_t *cgPhysicsCollider(bool enabled,
                                         bool dynamic,
                                         vec3s size,
                                         vec3s pos) {
  cg_physics_collider_t *collider = malloc(sizeof(cg_physics_collider_t));

  collider->enabled = enabled;
  collider->dynamic = dynamic;
  collider->size = size;
  collider->pos = pos;
  collider->stepHeight = 0.0f;

  return collider;
}

void cgPhysicsWorldAdd(cg_physics_world_t **physicsWorld,
                       cg_physics_collider_t *collider) {
  for (size_t i = 0; i < CG_PHYSICS_WORLD_MAX_OBJECTS; i++)
    if ((*physicsWorld)->colliders[i] == NULL) {
      (*physicsWorld)->colliders[i] = collider;
      break;
    }
}

vec3s cgPhysicsWorldAttemptMove(cg_physics_world_t *physicsWorld,
                                cg_physics_collider_t *collider,
                                vec3s move,
                                bool *outIsOnFloor) {
  float stepDelta = move.y;

  for (size_t i = 0; i < CG_PHYSICS_WORLD_MAX_OBJECTS; i++) {
    cg_physics_collider_t *objCollider = physicsWorld->colliders[i];

    if (objCollider == NULL)
      continue;
    if (memcmp(collider, objCollider, sizeof(cg_physics_collider_t)) == 0)
      continue;

    vec3s cldMaxPoints = {
      collider->pos.x + collider->size.x / 2.0f,
      collider->pos.y + collider->size.y / 2.0f,
      collider->pos.z + collider->size.z / 2.0f,
    };

    vec3s cldMinPoints = {
      collider->pos.x - collider->size.x / 2.0f,
      collider->pos.y - collider->size.y / 2.0f,
      collider->pos.z - collider->size.z / 2.0f,
    };

    vec3s objCldMaxPoints = {
      objCollider->pos.x + objCollider->size.x / 2.0f,
      objCollider->pos.y + objCollider->size.y / 2.0f,
      objCollider->pos.z + objCollider->size.z / 2.0f,
    };

    vec3s objCldMinPoints = {
      objCollider->pos.x - objCollider->size.x / 2.0f,
      objCollider->pos.y - objCollider->size.y / 2.0f,
      objCollider->pos.z - objCollider->size.z / 2.0f,
    };

    bool isOnAxisX =
      cldMaxPoints.x > objCldMinPoints.x
      && cldMinPoints.x < objCldMaxPoints.x;

    bool isOnAxisY =
      cldMaxPoints.y > objCldMinPoints.y
      && cldMinPoints.y < objCldMaxPoints.y;

    bool isOnAxisZ =
      cldMaxPoints.z > objCldMinPoints.z
      && cldMinPoints.z < objCldMaxPoints.z;

    // sus
    bool isPenetratingNegativeX =
      objCldMinPoints.x - (cldMaxPoints.x + move.x) < move.x;
    bool isPenetratingNegativeY =
      objCldMinPoints.y - (cldMinPoints.y + move.y) < move.y;
    bool isPenetratingNegativeZ =
      objCldMinPoints.z - (cldMaxPoints.z + move.z) < move.z;
    bool isPenetratingPositiveX =
      objCldMaxPoints.x - (cldMinPoints.x + move.x) > move.x;
    bool isPenetratingPositiveY =
      objCldMaxPoints.y - (cldMinPoints.y + move.y) > move.y;
    bool isPenetratingPositiveZ =
      objCldMaxPoints.z - (cldMinPoints.z + move.z) > move.z;

    if (isOnAxisY) {
      if (collider->pos.x < objCollider->pos.x)
        if (isPenetratingNegativeX && isOnAxisZ)
          move.x = objCldMinPoints.x - cldMaxPoints.x;

      if (collider->pos.x > objCollider->pos.x)
        if (isPenetratingPositiveX && isOnAxisZ)
          move.x = -(cldMinPoints.x - objCldMaxPoints.x);

      if (collider->pos.z < objCollider->pos.z)
        if (isPenetratingNegativeZ && isOnAxisX)
          move.z = objCldMinPoints.z - cldMaxPoints.z;

      if (collider->pos.z > objCollider->pos.z)
        if (isPenetratingPositiveZ && isOnAxisX)
          move.z = -(cldMinPoints.z - objCldMaxPoints.z);
    }
  }

  move.y = stepDelta;

  return move;
}

void cgPhysicsWorldDelete(cg_physics_world_t **physicsWorld) {
  for (size_t i = 0; i < CG_PHYSICS_WORLD_MAX_OBJECTS; i++)
    if ((*physicsWorld)->colliders[i] == NULL)
      free((*physicsWorld)->colliders[i]);

  free(*physicsWorld);
  *physicsWorld = NULL;
}
