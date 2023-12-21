#include "game.h"

#include <stddef.h>

#include "cglm/struct.h"

#include "main.h"
#include "player.h"
#include "mesh.h"
#include "shader.h"
#include "material.h"
#include "object.h"
#include "res.h"
#include "shaders.h"
#include "obj.h"
#include "physics.h"
#include "lights.h"
#include "prefabs.h"
#include "entity.h"

vec4s cgGameAmbientColor = { 0.0f, 0.0f, 0.0f, 1.0f };
cg_physics_world_t *cgGamePhysicsWorld = NULL;
cg_player_t *cgGamePlayer = NULL;

cg_object_t *cgGameWorldObj = NULL;
cg_entity_t *cgGameEntities[CG_GAME_ENTITIES_COUNT];

cg_start_func_t cgGameStart() {
  cgGamePhysicsWorld = cgPhysicsWorld();
  cgGamePlayer = cgPlayer();

  cgGameWorldObj = cgObjCreateObject("mdl/world.obj\0");
  cgPhysicsWorldAdd(&cgGamePhysicsWorld,
                    cgPhysicsCollider(true, false,
                                      (vec3s) { 20.0f, 1.0f, 20.0f },
                                      (vec3s) { 0.0f, -1.0f, 0.0f }));

  cgLightsPointAdd(0.0f, 5.0f, 0.0f,
                   0.75f, 1.0f, 1.5f,
                   10.0f);

  cgGameEntities[0] = cgEntity(2.0f, 0.0f, 3.0f, false);
}

cg_update_func_t cgGameUpdate() {
  cgPlayerUpdate(cgGamePlayer);

  for (size_t i = 0; i < CG_GAME_ENTITIES_COUNT; i++)
    ENTUPD(cgGameEntities[i]);
}

cg_draw_func_t cgGameDraw() {
  DRAWOBJ(cgGameWorldObj);

  for (size_t i = 0; i < CG_GAME_ENTITIES_COUNT; i++)
    DRAWOBJ(cgGameEntities[i]->object);
}

cg_end_func_t cgGameEnd() {
  cgPhysicsWorldDelete(&cgGamePhysicsWorld);
  cgPlayerDelete(&cgGamePlayer);
  cgObjectDelete(&cgGameWorldObj);

  for (size_t i = 0; i < CG_GAME_ENTITIES_COUNT; i++)
    cgEntityDelete(&cgGameEntities[i]);
}
