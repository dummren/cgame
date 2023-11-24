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

vec4s cgGameAmbientColor = { 0.0f, 0.0f, 0.0f, 1.0f };
cg_physics_world_t *cgGamePhysicsWorld = NULL;
cg_player_t *cgGamePlayer = NULL;

cg_object_t *trees[4];
cg_object_t *ground;

void makeObjects() {
  for (size_t i = 0; i < 4; i++)
    trees[i] = cgObjCreateObject("mdl/test_tree.obj\0");

  trees[0]->pos = (vec3s) { -1.0f, 0.0f, -1.0f };
  trees[1]->pos = (vec3s) { 1.0f, 0.0f, -1.0f };
  trees[2]->pos = (vec3s) { -1.0f, 0.0f, 1.0f };
  trees[3]->pos = (vec3s) { 1.0f, 0.0f, 1.0f };

  trees[2]->rot = (vec3s) { 0.0f, 3.14f, 0.0f };

  ground = cgObjCreateObject("mdl/test_ground.obj\0");

  cgPhysicsWorldAdd(&cgGamePhysicsWorld,
                    cgPhysicsCollider(true, false,
                                      (vec3s) { 0.25f, 0.25f, 0.25f },
                                      (vec3s) { -1.0f, 0.25f, -1.0f }));

  cgPhysicsWorldAdd(&cgGamePhysicsWorld,
                    cgPhysicsCollider(true, false,
                                      (vec3s) { 0.25f, 0.25f, 0.25f },
                                      (vec3s) { 1.0f, 0.25f, -1.0f }));

  cgPhysicsWorldAdd(&cgGamePhysicsWorld,
                    cgPhysicsCollider(true, false,
                                      (vec3s) { 0.25f, 0.25f, 0.25f },
                                      (vec3s) { -1.0f, 0.25f, 1.0f }));

  cgPhysicsWorldAdd(&cgGamePhysicsWorld,
                    cgPhysicsCollider(true, false,
                                      (vec3s) { 0.25f, 0.25f, 0.25f },
                                      (vec3s) { 1.0f, 0.25f, 1.0f }));
}

cg_start_func_t cgGameStart() {
  cgGamePhysicsWorld = cgPhysicsWorld();
  cgGamePlayer = cgPlayer();
  makeObjects();
}

cg_update_func_t cgGameUpdate() {
  cgPlayerUpdate(cgGamePlayer);
}

cg_draw_func_t cgGameDraw() {
  cgLightsPoint(0.0f, 3.5f, 0.0f,
                1.5f, 0.0f, 0.0f,
                10.0f);
  cgLightsPoint(10.0f, 5.0f, 5.0f,
                1.2f, 1.2f, 1.2f,
                10.0f);

  for (size_t i = 0; i < 4; i++)
    cgObjectDraw(trees[i],
                 cgCameraProj(cgGamePlayer->cam),
                 cgPlayerView(cgGamePlayer));

  cgObjectDraw(ground, cgCameraProj(cgGamePlayer->cam),
               cgPlayerView(cgGamePlayer));
}

cg_end_func_t cgGameEnd() {
  cgPhysicsWorldDelete(&cgGamePhysicsWorld);
  cgPlayerDelete(&cgGamePlayer);

  for (size_t i = 0; i < 4; i++)
    cgObjectDelete(&trees[i]);

  cgObjectDelete(&ground);
}
