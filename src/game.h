#ifndef __CG__GAME_H__
#define __CG__GAME_H__

#include "cglm/struct.h"

#include "main.h"
#include "player.h"
#include "physics.h"
#include "entity.h"

#define _DRAWOBJARR(...) (cg_object_t*[]) { __VA_ARGS__ }
#define _DRAWOBJLEN(...) sizeof(_DRAWOBJARR(__VA_ARGS__)) / sizeof(cg_object_t*)
#define DRAWOBJ(object) cgObjectDraw(object,                          \
                                     cgCameraProj(cgGamePlayer->cam), \
                                     cgPlayerView(cgGamePlayer))
#define DRAWOBJS(...) for (size_t i = 0; i < _DRAWOBJLEN(__VA_ARGS__); i++) \
    DRAWOBJ(_DRAWOBJARR(__VA_ARGS__)[i])

extern vec4s cgGameAmbientColor;
extern cg_physics_world_t *cgGamePhysicsWorld;
extern cg_player_t *cgGamePlayer;

extern cg_object_t *cgGameWorldObj;
#define CG_GAME_ENTITIES_COUNT 1
extern cg_entity_t *cgGameEntities[CG_GAME_ENTITIES_COUNT];

cg_start_func_t cgGameStart();
cg_update_func_t cgGameUpdate();
cg_draw_func_t cgGameDraw();
cg_end_func_t cgGameEnd();

#endif // __CG__GAME_H__
