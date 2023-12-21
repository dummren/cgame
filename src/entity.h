#ifndef __CG__ENTITY_H__
#define __CG__ENTITY_H__

#include <math.h>
#include <stdbool.h>

#include "object.h"

#define ENT2PLAYER_DIST_MIN 1.0f
#define ENT_FRIEND_BRIGHTCOL (vec3s) { 0.0f, 1.2f, 0.0f }
#define ENT_NFRIEND_BRIGHTCOL (vec3s) { 0.4f, 0.0f, 0.0f }

typedef struct {
  cg_object_t *object;
  bool isFriendly;
} cg_entity_t;

#define _ENTUPDARR(...) (cg_entity_t*[]) { __VA_ARGS__ }
#define _ENTUPDLEN(...) sizeof(_ENTUPDARR(__VA_ARGS__)) / sizeof(cg_entity_t*)
#define ENTUPD(entity) { \
    entity->object->pos.x += sin(glfwGetTime()) * 0.001f;               \
    entity->object->pos.y += cos(glfwGetTime()) * 0.001f;               \
    entity->object->pos.z += sin(glfwGetTime() * cos(glfwGetTime())) * 0.001f; \
    cgEntityUpdateColor(entity); }
#define ENTSUPD(...) for (size_t i = 0; i < _ENTUPDLEN(__VA_ARGS__); i++) \
    ENTUPD(_ENTUPDARR(__VA_ARGS__)[i])

cg_entity_t *cgEntity(const float, const float, const float, const bool);
void cgEntityUpdateColor(cg_entity_t*);
void cgEntityDelete(cg_entity_t**);

#endif // __CG__ENTITY_H__
