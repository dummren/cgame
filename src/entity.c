#include "entity.h"

#include <stdbool.h>

#include "cglm/struct.h"

#include "object.h"
#include "prefabs.h"
#include "game.h"

cg_entity_t *cgEntity(const float x, const float y, const float z,
                      const bool isFriendly) {
  cg_entity_t *entity = malloc(sizeof(cg_entity_t));

  entity->object = cgPrefabsEntity();
  entity->object->pos = (vec3s) { x, y, z };
  entity->isFriendly = isFriendly;

  return entity;
}

void cgEntityUpdateColor(cg_entity_t *entity) {
  vec3s entPos = entity->object->pos;
  vec3s playerPos = cgGamePlayer->pos;

  entPos.y = 0.0f;
  playerPos.y = 0.0f;

  float dist2player = glms_vec3_distance(entPos, playerPos);

  entity->object->materials[0]->brightCol =
    dist2player > ENT2PLAYER_DIST_MIN ?
    (entity->isFriendly ? ENT_FRIEND_BRIGHTCOL : ENT_NFRIEND_BRIGHTCOL)
    : (vec3s) { 0.0f, 0.0f, 0.0f };
}

void cgEntityDelete(cg_entity_t **entity) {
  cgObjectDelete(&(*entity)->object);
  free(*entity);
  *entity = NULL;
}
