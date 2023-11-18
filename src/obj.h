#ifndef __CG__OBJ_H__
#define __CG__OBJ_H__

#include <stddef.h>
#include <stdbool.h>

#include "material.h"
#include "object.h"

// note that all textures filepaths format be data/tex/{filepath}
void cgObjReadMtl(const char*, size_t*, cg_material_t**, char**);

// only supports meshes exported from blender in .obj with default options
// + triangulated mesh option
// + strp paths option
// (support up to CG_OBJECT_MAX_MATERIALS materials)
// (and also used .mtl files should be located in data/mdl/)
// :)
void cgObjRead(const char*,
               unsigned int*, unsigned int*,
               float*, float*, float*,
               size_t*, size_t*, size_t*, size_t*, size_t*,
               cg_material_t**);

cg_object_t *cgObjCreateObject(const char*);

#endif // __CG__OBJ_H__
