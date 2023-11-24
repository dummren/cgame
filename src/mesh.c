#include "mesh.h"

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "glcore.h"

cg_mesh_t *cgMesh() {
  cg_mesh_t *mesh = malloc(sizeof(cg_mesh_t));
  glGenVertexArrays(1, &mesh->vao);

  return mesh;
}

void cgMeshBind(const cg_mesh_t *mesh) {
  glBindVertexArray(mesh->vao);
}

void cgMeshUnbind() {
  glBindVertexArray(0);
}

void cgMeshStoref(const cg_mesh_t *mesh,
                  const float *data,
                  const size_t dataSize,
                  const unsigned int pos,
                  const int size) {
  unsigned int buffer;
  glGenBuffers(1, &buffer);

  cgMeshBind(mesh);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);

  glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
  glEnableVertexAttribArray(pos);
  glVertexAttribPointer(pos, size, GL_FLOAT, false, 0, NULL);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  cgMeshUnbind();

  glDeleteBuffers(1, &buffer);
}

void cgMeshStoreuint(const cg_mesh_t *mesh,
                     const unsigned int *data,
                     const size_t dataSize,
                     const unsigned int pos,
                     const int size) {
  unsigned int buffer;
  glGenBuffers(1, &buffer);

  cgMeshBind(mesh);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);

  glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
  glEnableVertexAttribArray(pos);
  glVertexAttribIPointer(pos, size, GL_UNSIGNED_INT, 0, NULL);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  cgMeshUnbind();

  glDeleteBuffers(1, &buffer);
}

void cgMeshStoreIndices(cg_mesh_t *mesh,
                        const unsigned int *data,
                        const size_t dataSize,
                        const unsigned int pos) {
  cgMeshStoreuint(mesh, data, dataSize, pos, 1);

  glGenBuffers(1, &mesh->ebo);
  mesh->eboLength = dataSize / sizeof(unsigned int);

  cgMeshBind(mesh);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
  cgMeshUnbind();
}

void cgMeshDraw(const cg_mesh_t *mesh) {
  cgMeshBind(mesh);
  glDrawElements(GL_TRIANGLES, mesh->eboLength, GL_UNSIGNED_INT, NULL);
  cgMeshUnbind(mesh);
}

void cgMeshDelete(cg_mesh_t **mesh) {
  glDeleteVertexArrays(1, &(*mesh)->vao);
  glDeleteBuffers(1, &(*mesh)->ebo);
  free(*mesh);
  *mesh = NULL;
}
