#include "prefabs.h"

#include <stdlib.h>
#include <string.h>

#include "object.h"
#include "obj.h"

static cg_object_t *entity = NULL;

void cgPrefabsInit() {
  entity = cgObjCreateObject("mdl/entity.obj\0");
}

cg_object_t *cgPrefabsEntity() {
  cg_object_t *object = malloc(sizeof(cg_object_t));
  memcpy(object, entity, sizeof(cg_object_t));

  return object;
}
