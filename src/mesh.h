#ifndef __CG__MESH_H__
#define __CG__MESH_H__

#include <stddef.h>

typedef struct {
  unsigned int vao;
  unsigned int ebo;
  size_t eboLength;
} cg_mesh_t;

cg_mesh_t *cgMesh();

void cgMeshBind(const cg_mesh_t*);
void cgMeshUnbind();

void cgMeshStoref(const cg_mesh_t*,
                  const float*,
                  const size_t,
                  const unsigned int,
                  const int);
void cgMeshStoreuint(const cg_mesh_t*,
                     const unsigned int*,
                     const size_t,
                     const unsigned int,
                     const int);
void cgMeshStoreIndices(cg_mesh_t*,
                        const unsigned int*,
                        const size_t,
                        const unsigned int);

void cgMeshDraw(const cg_mesh_t*);

void cgMeshDelete(cg_mesh_t**);

#endif // __CG__MESH_H__
