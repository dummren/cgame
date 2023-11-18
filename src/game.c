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

cg_player_t *cgGamePlayer = NULL;

cg_object_t *trees[4];
cg_object_t *ground;

void makePon() {
  for (size_t i = 0; i < 4; i++)
    trees[i] = cgObjCreateObject("mdl/test_tree.obj\0");

  trees[0]->pos = (vec3s) { -1.0f, 0.0f, -1.0f };
  trees[1]->pos = (vec3s) { 1.0f, 0.0f, -1.0f };
  trees[2]->pos = (vec3s) { -1.0f, 0.0f, 1.0f };
  trees[3]->pos = (vec3s) { 1.0f, 0.0f, 1.0f };

  trees[2]->rot = (vec3s) { 1.54f, 0.44f, 145.4f };

  ground = cgObjCreateObject("mdl/test_ground.obj\0");
}

cg_start_func_t cgGameStart() {
  cgGamePlayer = cgPlayer();
  makePon();
}

cg_update_func_t cgGameUpdate() {
  cgPlayerUpdate(cgGamePlayer);
}

cg_draw_func_t cgGameDraw() {
  for (size_t i = 0; i < 4; i++)
    cgObjectDraw(trees[i],
                 cgCameraProj(cgGamePlayer->cam),
                 cgPlayerView(cgGamePlayer));

  cgObjectDraw(ground, cgCameraProj(cgGamePlayer->cam),
               cgPlayerView(cgGamePlayer));
}

cg_end_func_t cgGameEnd() {
  free(cgGamePlayer);
}
