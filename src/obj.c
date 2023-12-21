///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// WARNING // WARNING // WARNING // WARNING // WARNING // WARNING // WARNING //
// WARNING // WARNING // WARNING // WARNING // WARNING // WARNING // WARNING //
// WARNING // WARNING // WARNING // WARNING // WARNING // WARNING // WARNING //
// WARNING // WARNING // WARNING // WARNING // WARNING // WARNING // WARNING //
// WARNING // WARNING // WARNING // WARNING // WARNING // WARNING // WARNING //
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//     please DO NOT try to understand how does cgObjLoad function work      //
//                          DO NOT even look at it                           //
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// WARNING // WARNING // WARNING // WARNING // WARNING // WARNING // WARNING //
// WARNING // WARNING // WARNING // WARNING // WARNING // WARNING // WARNING //
// WARNING // WARNING // WARNING // WARNING // WARNING // WARNING // WARNING //
// WARNING // WARNING // WARNING // WARNING // WARNING // WARNING // WARNING //
// WARNING // WARNING // WARNING // WARNING // WARNING // WARNING // WARNING //
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "obj.h"

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "glcore.h"
#include "material.h"
#include "texture.h"
#include "res.h"
#include "utils.h"
#include "settings.h"
#include "shaders.h"
#include "mesh.h"
#include "consts.h"

static unsigned int *f_adduint(unsigned int *data, size_t *dataOldLen,
                               const unsigned int *values,
                               size_t count, size_t stride) {
  data = realloc(data, sizeof(unsigned int) * (*dataOldLen + count));

  for (size_t i = 0; i < count; i++)
    data[(*dataOldLen)++] = values[i * stride];

  return data;
}

static float *f_addf(float *data, size_t *dataOldLen,
                     const float *values,
                     size_t count, size_t stride) {
  data = realloc(data, sizeof(float) * (*dataOldLen + count));

  for (size_t i = 0; i < count; i++)
    data[(*dataOldLen)++] = values[i * stride];

  return data;
}

void cgObjReadMtl(const char *path, size_t *outCount,
                  cg_material_t **outMaterials, char **outMaterialsNames) {
  char mtlContent[cgResLen(path)];
  cgResRead(path, mtlContent);

  size_t linesCount = cgUtilsStrCount(mtlContent, "\n\0");
  char *lines[linesCount];
  cgUtilsStrSplit(mtlContent, "\n\0", lines);

  cg_material_t *material = NULL;

  for (size_t i = 0; i < linesCount; i++) {
    char *line = lines[i];

    if (cgUtilsStrStartsWith(line, "newmtl \0")) {
      char materialName[strlen(line)];
      strcpy(materialName, line + strlen("newmtl \0"));

      if (outCount)
        (*outCount)++;

      if (material)
        if (outMaterials)
          *(outMaterials++) = material;

      if (outMaterialsNames) {
        *outMaterialsNames = malloc(strlen(materialName) + 1);
        strcpy(*outMaterialsNames, materialName);
        outMaterialsNames++;
      }

      material = cgMaterial();
    } else if (cgUtilsStrStartsWith(line, "Kd \0")) {
      sscanf(line, "Kd %f %f %f\0",
             &material->albedoCol.r,
             &material->albedoCol.g,
             &material->albedoCol.b);
    } else if (cgUtilsStrStartsWith(line, "map_Kd \0")) {
      int spacePos = cgUtilsStrFind(line, " \0", 0);
      char *texname = cgUtilsStrSub(line,
                                    spacePos + 1,
                                    strlen(line) - (spacePos + 1));

      char texpath[strlen("tex/\0") + strlen(texname) + 1];
      strcpy(texpath, "tex/\0");
      strcat(texpath, texname);

      int textureContentLen = cgResLen(texpath);
      unsigned char textureContent[textureContentLen];
      cgResRead(texpath, textureContent);

      material->albedoTexEnabled = true;
      material->albedoTex =
        cgTextureLoadFromMemory(textureContent,
                                GL_UNSIGNED_BYTE,
                                textureContentLen,
                                NULL, NULL,
                                NULL,
                                4,
                                TEX_FMT, GL_RGBA,
                                GL_REPEAT, GL_REPEAT,
                                CGS_TEX_FLTR, CGS_TEX_FLTR);

      free(texname);
    }
  }

  *outMaterials = material; // push the last material
}

void cgObjRead(const char *path,
               unsigned int *outIndices, unsigned int *outMaterialsIndices,
               float *outPositions, float *outNormals, float *outUv,
               size_t *outIndicesLen, size_t *outMaterialsIndicesLen,
               size_t *outPositionsLen,
               size_t *outNormalsLen, size_t *outUvLen,
               cg_material_t **outMaterials) {
  char objContent[cgResLen(path)];
  cgResRead(path, objContent);

  size_t linesCount = cgUtilsStrCount(objContent, "\n\0");
  char *lines[linesCount];
  cgUtilsStrSplit(objContent, "\n\0", lines);

  // yes this part of the code is so cursed and compilcated for me
  // so i added comments just not to get lost in it

  // data stored under v, vn and vt tags
  size_t positionsUnsortedLen = 0;
  size_t normalsUnsortedLen = 0;
  size_t uvUnsortedLen = 0;

  float *positionsUnsorted = malloc(0);
  float *normalsUnsorted = malloc(0);
  float *uvUnsorted = malloc(0);

  // data stored under f tag
  size_t positionsIndicesLen = 0;
  size_t normalsIndicesLen = 0;
  size_t uvIndicesLen = 0;

  unsigned int *positionsIndices = malloc(0);
  unsigned int *normalsIndices = malloc(0);
  unsigned int *uvIndices = malloc(0);

  // custom out data used by the engine
  size_t indicesLen = 0;
  size_t materialsIndicesLen = 0;
  size_t positionsLen = 0;
  size_t normalsLen = 0;
  size_t uvLen = 0;

  unsigned int *indices = malloc(0);
  unsigned int *materialsIndices = malloc(0);
  float *positions = malloc(0);
  float *normals = malloc(0);
  float *uv = malloc(0);

  cg_material_t *materials[CG_OBJECT_MAX_MATERIALS];
  char *materialsNames[CG_OBJECT_MAX_MATERIALS];
  size_t materialsCount = 0;
  size_t currentMaterial = 0;

  // parsing data from file
  for (size_t i = 0; i < linesCount; i++) {
    char *line = lines[i];

    if (cgUtilsStrStartsWith(line, "v \0")) {
      float values[3];
      sscanf(line, "v %f %f %f\0", values, values + 1, values + 2);
      positionsUnsorted = f_addf(positionsUnsorted, &positionsUnsortedLen,
                                 values,
                                 3, 1);
    } else if (cgUtilsStrStartsWith(line, "vt \0")) {
      float values[2];
      sscanf(line, "vt %f %f\0", values, values + 1);
      uvUnsorted = f_addf(uvUnsorted, &uvUnsortedLen,
                          values,
                          2, 1);
    } else if (cgUtilsStrStartsWith(line, "vn \0")) {
      float values[3];
      sscanf(line, "vn %f %f %f\0", values, values + 1, values + 2);
      normalsUnsorted = f_addf(normalsUnsorted, &normalsUnsortedLen,
                               values,
                               3, 1);
    } else if (cgUtilsStrStartsWith(line, "f \0")) {
      unsigned int values[9];

      sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d\0",
             values + 0, values + 1, values + 2,
             values + 3, values + 4, values + 5,
             values + 6, values + 7, values + 8);

      positionsIndices = f_adduint(positionsIndices, &positionsIndicesLen,
                                   values,
                                   3, 3);
      uvIndices = f_adduint(uvIndices, &uvIndicesLen,
                            values + 1,
                            3, 3);
      normalsIndices = f_adduint(normalsIndices, &normalsIndicesLen,
                                 values + 2,
                                 3, 3);

      unsigned int index = currentMaterial;
      materialsIndices = f_adduint(materialsIndices, &materialsIndicesLen,
                                   &index,
                                   3, 0);
    } else if (cgUtilsStrStartsWith(line, "mtllib \0")) {
      int spacePos = cgUtilsStrFind(line, " \0", 0);
      char *libname = cgUtilsStrSub(line,
                                    spacePos + 1,
                                    strlen(line) - (spacePos + 1));

      char *libpath = malloc(strlen("mdl/\0") + strlen(libname) + 1);
      strcpy(libpath, "mdl/\0");
      strcat(libpath, libname);

      cgObjReadMtl(libpath, &materialsCount, materials, materialsNames);
    } else if (cgUtilsStrStartsWith(line, "usemtl \0")) {
      int spacePos = cgUtilsStrFind(line, " \0", 0);
      char *mtlname = cgUtilsStrSub(line,
                                    spacePos + 1,
                                    strlen(line) - (spacePos + 1));

      for (size_t i = 0; i < materialsCount; i++)
        if (strcmp(materialsNames[i], mtlname) == 0) {
          currentMaterial = i;
          break;
        }

      free(mtlname);
    }
  }

  // converting data for in-game usage
  for (size_t i = 0; i < positionsIndicesLen; i++) {
    unsigned int ind[] = { i };

    indices = f_adduint(indices, &indicesLen,
                        ind,
                        1, 0);
  }

  for (size_t i = 0; i < positionsIndicesLen; i++) {
    size_t start = 3 * (positionsIndices[i] - 1);

    positions = f_addf(positions, &positionsLen,
                       positionsUnsorted + start,
                       3, 1);
  }

  for (size_t i = 0; i < normalsIndicesLen; i++) {
    size_t start = 3 * (normalsIndices[i] - 1);

    normals = f_addf(normals, &normalsLen,
                     normalsUnsorted + start,
                     3, 1);
  }

  for (size_t i = 0; i < uvIndicesLen; i++) {
    size_t start = 2 * (uvIndices[i] - 1);

    uv = f_addf(uv, &uvLen,
                uvUnsorted + start,
                2, 1);
  }

  // copy data
  if (outIndices)
    for (size_t i = 0; i < indicesLen; i++)
      outIndices[i] = indices[i];
  if (outMaterialsIndices)
    for (size_t i = 0; i < materialsIndicesLen; i++)
      outMaterialsIndices[i] = materialsIndices[i];
  if (outPositions)
    for (size_t i = 0; i < positionsLen; i++)
      outPositions[i] = positions[i];
  if (outNormals)
    for (size_t i = 0; i < normalsLen; i++)
      outNormals[i] = normals[i];
  if (outUv)
    for (size_t i = 0; i < uvLen; i++)
      outUv[i] = uv[i];

  if (outIndicesLen)
    *outIndicesLen = indicesLen;
  if (outMaterialsIndicesLen)
    *outMaterialsIndicesLen = materialsIndicesLen;
  if (outPositionsLen)
    *outPositionsLen = positionsLen;
  if (outNormalsLen)
    *outNormalsLen = normalsLen;
  if (outUvLen)
    *outUvLen = uvLen;

  if (outMaterials)
    for (size_t i = 0; i < materialsCount; i++)
      *(outMaterials++) = materials[i];

  // free
  for (size_t i = 0; i < linesCount; i++)
    free(lines[i]);

  free(positionsUnsorted);
  free(normalsUnsorted);
  free(uvUnsorted);
  free(positionsIndices);
  free(normalsIndices);
  free(uvIndices);
  free(indices);
  free(positions);
  free(normals);
  free(uv);

  // it was commented lmao
  for (size_t i = 0; i < materialsCount; i++)
    free(materialsNames[i]);
}

cg_object_t *cgObjCreateObject(const char *path) {
  size_t indicesLen, materialsIndicesLen,
    positionsLen, normalsLen, uvLen;

  cg_material_t *materials[CG_OBJECT_MAX_MATERIALS];
  for (size_t i = 0; i < CG_OBJECT_MAX_MATERIALS; i++)
    materials[i] = cgMaterial();

  cgObjRead(path,
            NULL, NULL, NULL, NULL, NULL,
            &indicesLen, &materialsIndicesLen,
            &positionsLen, &normalsLen, &uvLen,
            materials);

  unsigned int indices[indicesLen];
  unsigned int materialsIndices[materialsIndicesLen];
  float positions[positionsLen];
  float normals[normalsLen];
  float uv[uvLen];

  cgObjRead(path,
            indices, materialsIndices, positions, normals, uv,
            NULL, NULL, NULL, NULL, NULL,
            NULL);

  cg_mesh_t *mesh = cgMesh();
  cgMeshStoreIndices(mesh, indices, sizeof(indices), 0);
  cgMeshStoreuint(mesh, materialsIndices, sizeof(materialsIndices), 1, 1);
  cgMeshStoref(mesh, positions, sizeof(positions), 2, 3);
  cgMeshStoref(mesh, normals, sizeof(normals), 3, 3);
  cgMeshStoref(mesh, uv, sizeof(uv), 4, 2);

  cg_object_t *object = cgObjectMake(cgShadersObjectProg, mesh, materials);
  return object;
}
