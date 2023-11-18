#include "object.h"

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "cglm/struct.h"

#include "glcore.h"
#include "main.h"
#include "shader.h"
#include "material.h"
#include "mesh.h"
#include "camera.h"

cg_object_t *cgObject() {
  cg_object_t *object = malloc(sizeof(cg_object_t));

  object->prog = 0;
  object->mesh = cgMesh();

  for (size_t i = 0; i < CG_OBJECT_MAX_MATERIALS; i++)
    object->materials[i] = cgMaterial();

  object->materialSlots = malloc(0);

  object->pos = (vec3s) { 0.0f, 0.0f, 0.0f };
  object->rot = (vec3s) { 0.0f, 0.0f, 0.0f };

  return object;
}

cg_object_t *cgObjectMake(cg_shader_prog_t prog,
                          cg_mesh_t *mesh,
                          cg_material_t **materials) {
  cg_object_t *object = cgObject();

  object->prog = prog;
  object->mesh = mesh;

  if (materials)
    for (size_t i = 0; i < CG_OBJECT_MAX_MATERIALS; i++)
      object->materials[i] = materials[i];

  object->materialSlots = malloc(sizeof(unsigned int) * mesh->eboLength);

  return object;
}

mat4s cgObjectTrans(cg_object_t *object) {
  mat4s trans = GLMS_MAT4_IDENTITY;

  trans = glms_translate(trans, object->pos);
  trans = glms_rotate(trans, object->rot.x, GLMS_XUP);
  trans = glms_rotate(trans, object->rot.y, GLMS_YUP);
  trans = glms_rotate(trans, object->rot.z, GLMS_ZUP);

  return trans;
}

void cgObjectDraw(cg_object_t *object, mat4s projMat, mat4s viewMat) {
  cgShaderProgBind(object->prog);

  for (size_t i = 0; i < CG_OBJECT_MAX_MATERIALS; i++) {
    cg_material_t *material = object->materials[i];

    glActiveTexture(GL_TEXTURE0 + i * 2 + 0);
    cgTextureBind(material->albedoTex);

    glActiveTexture(GL_TEXTURE0 + i * 2 + 1);
    cgTextureBind(material->brightTex);

    char uName[128];
    char uAlbedoColName[128];
    char uBrightColName[128];
    char uAlbedoTexEnabledName[128];
    char uBrightTexEnabledName[128];
    char uAlbedoTexName[128];
    char uBrightTexName[128];

    memset(uName, 0, sizeof(uName));
    memset(uAlbedoColName, 0, sizeof(uAlbedoColName));
    memset(uBrightColName, 0, sizeof(uBrightColName));
    memset(uAlbedoTexEnabledName, 0, sizeof(uAlbedoTexEnabledName));
    memset(uBrightTexEnabledName, 0, sizeof(uBrightTexEnabledName));
    memset(uAlbedoTexName, 0, sizeof(uAlbedoTexName));
    memset(uBrightTexName, 0, sizeof(uBrightTexName));

    {
      char istr[33];
      memset(istr, 0, sizeof(istr));

      sprintf(istr, "%d\0", i);

      strcpy(uName, "u_materials[\0");
      strcat(uName, istr);
      strcat(uName, "]\0");
    }

    strcpy(uAlbedoColName, uName);
    strcpy(uBrightColName, uName);
    strcpy(uAlbedoTexEnabledName, uName);
    strcpy(uBrightTexEnabledName, uName);
    strcpy(uAlbedoTexName, uName);
    strcpy(uBrightTexName, uName);

    strcat(uAlbedoColName, ".albedoCol\0");
    strcat(uBrightColName, ".brightCol\0");
    strcat(uAlbedoTexEnabledName, ".albedoTexEnabled\0");
    strcat(uBrightTexEnabledName, ".brightTexEnabled\0");
    strcat(uAlbedoTexName, ".albedoTex\0");
    strcat(uBrightTexName, ".brightTex\0");

    vec4s brightCol = (vec4s) {
      material->brightCol.r,
      material->brightCol.g,
      material->brightCol.b,
      1.0f
    };

    glUniform4fv(glGetUniformLocation(object->prog, uAlbedoColName),
                 1, material->albedoCol.raw);
    glUniform4fv(glGetUniformLocation(object->prog, uBrightColName),
                 1, brightCol.raw);

    glUniform1i(glGetUniformLocation(object->prog, uAlbedoTexEnabledName),
                material->albedoTexEnabled);
    glUniform1i(glGetUniformLocation(object->prog, uBrightTexEnabledName),
                material->brightTexEnabled);

    glUniform1i(glGetUniformLocation(object->prog, uAlbedoTexName), i * 2 + 0);
    glUniform1i(glGetUniformLocation(object->prog, uBrightTexName), i * 2 + 1);
  }

  glUniform1f(glGetUniformLocation(object->prog, "u_time"), glfwGetTime());
  glUniformMatrix4fv(glGetUniformLocation(object->prog, "u_projMat"),
                     1, false, projMat.raw[0]);
  glUniformMatrix4fv(glGetUniformLocation(object->prog, "u_viewMat"),
                     1, false, viewMat.raw[0]);
  glUniformMatrix4fv(glGetUniformLocation(object->prog, "u_transMat"),
                     1, false, cgObjectTrans(object).raw[0]);

  cgMeshBind(object->mesh);
  cgMeshDraw(object->mesh);
  cgMeshUnbind();

  for (size_t i = 0; i < CG_OBJECT_MAX_MATERIALS; i++) {
    glActiveTexture(GL_TEXTURE0 + i * 2 + 0);
    cgTextureUnbind();

    glActiveTexture(GL_TEXTURE0 + i * 2 + 1);
    cgTextureUnbind();
  }
}

#pragma message "TODO: cleanup object"
