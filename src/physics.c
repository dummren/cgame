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
                                vec3s move) {
  bool isMaxFloorRelYSet = false;
  float maxFloorRelY = 0.0f;

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

    bool isCloserToNegativeX = collider->pos.x < objCollider->pos.x;
    bool isCloserToPositiveX = collider->pos.x > objCollider->pos.x;
    bool isCloserToNegativeY = collider->pos.y < objCollider->pos.y;
    bool isCloserToPositiveY = collider->pos.y > objCollider->pos.y;
    bool isCloserToNegativeZ = collider->pos.z < objCollider->pos.z;
    bool isCloserToPositiveZ = collider->pos.z > objCollider->pos.z;

    bool isInsideY = (isPenetratingNegativeX && isPenetratingPositiveX)
      && (isPenetratingNegativeZ && isPenetratingPositiveZ);

    if (collider->dynamic && isInsideY) {
      float objRelY = objCldMaxPoints.y - cldMinPoints.y;

      if ((objRelY > maxFloorRelY || !isMaxFloorRelYSet)
          && fabs(objRelY) <= collider->stepHeight) {
        maxFloorRelY = objRelY;
        isMaxFloorRelYSet = true;
      }
    }

    if (isOnAxisY) {
      if (isPenetratingNegativeX && isOnAxisZ && isCloserToNegativeX) {
        float pushDistance = objCldMinPoints.x - cldMaxPoints.x;

        if (fabs(pushDistance) <= fabs(move.x))
          move.x = pushDistance;
        else
          break;
      }

      if (isPenetratingPositiveX && isOnAxisZ && isCloserToPositiveX) {
        float pushDistance = -(cldMinPoints.x - objCldMaxPoints.x);

        if (fabs(pushDistance) <= fabs(move.x))
          move.x = pushDistance;
        else
          break;
      }

      if (isPenetratingNegativeZ && isOnAxisX && isCloserToNegativeZ) {
        float pushDistance = objCldMinPoints.z - cldMaxPoints.z;

        if (fabs(pushDistance) <= fabs(move.z))
          move.z = pushDistance;
        else
          break;
      }

      if (isPenetratingPositiveZ && isOnAxisX && isCloserToPositiveZ) {
        float pushDistance = -(cldMinPoints.z - objCldMaxPoints.z);

        if (fabs(pushDistance) <= fabs(move.z))
          move.z = pushDistance;
        else
          break;
      }
    }
  }

  move.y = maxFloorRelY;

  return move;
}

bool cgPhysicsIsPointInsideBox(cg_physics_world_t *physicsWorld,
                               vec3s pointPos,
                               vec3s boxPos, vec3s boxSize) {
  return false;
}

void cgPhysicsWorldDelete(cg_physics_world_t **physicsWorld) {
  for (size_t i = 0; i < CG_PHYSICS_WORLD_MAX_OBJECTS; i++)
    if ((*physicsWorld)->colliders[i] == NULL)
      free((*physicsWorld)->colliders[i]);

  free(*physicsWorld);
  *physicsWorld = NULL;
}
