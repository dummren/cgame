#ifndef __CG__GAME_H__
#define __CG__GAME_H__

#include <cglm/struct.h>

#include "main.h"
#include "player.h"
#include "physics.h"

extern vec4s cgGameAmbientColor;
extern cg_physics_world_t *cgGamePhysicsWorld;
extern cg_player_t *cgGamePlayer;

cg_start_func_t cgGameStart();
cg_update_func_t cgGameUpdate();
cg_draw_func_t cgGameDraw();
cg_end_func_t cgGameEnd();

#endif // __CG__GAME_H__
