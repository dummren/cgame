#include "texture.h"

#include "stb/stb_image.h"

#include "glcore.h"

cg_texture_t cgTextureEmpty(const unsigned int type,
                            const int width, const int height,
                            const int internalFormat, const int format,
                            const int wrapS, const int wrapT,
                            const int minFltr, const int magFltr) {
  return cgTextureLoad(NULL,
                       type,
                       width, height,
                       internalFormat, format,
                       wrapS, wrapT,
                       minFltr, magFltr);
}

cg_texture_t cgTextureLoad(const unsigned char *glData,
                           const unsigned int type,
                           const int width, const int height,
                           const int internalFormat, const int format,
                           const int wrapS, const int wrapT,
                           const int minFltr, const int magFltr) {
  cg_texture_t texture;
  glGenTextures(1, &texture);
  cgTextureBind(texture);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFltr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFltr);

  glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format,
               type, glData);
  glGenerateMipmap(GL_TEXTURE_2D);

  cgTextureUnbind();
  return texture;
}

cg_texture_t cgTextureLoadFromMemory(const unsigned char *data,
                                     const unsigned int type,
                                     const int len,
                                     int *outWidth, int *outHeight,
                                     int *outChannels,
                                     const int desiredChannels,
                                     const int internalFormat,
                                     const int format,
                                     const int wrapS, const int wrapT,
                                     const int minFltr, const int magFltr) {
  cg_texture_t texture;
  glGenTextures(1, &texture);

  int width, height, channels;
  unsigned char *glData = stbi_load_from_memory(data, len,
                                                &width, &height,
                                                &channels, desiredChannels);

  if (outWidth)
    *outWidth = width;
  if (outHeight)
    *outHeight = height;
  if (outChannels)
    *outChannels = channels;

  cgTextureBind(texture);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFltr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFltr);

  glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format,
               type, glData);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(glData);
  cgTextureUnbind();

  return texture;
}

void cgTextureBind(const cg_texture_t texture) {
  glBindTexture(GL_TEXTURE_2D, texture);
}

void cgTextureUnbind() {
  glBindTexture(GL_TEXTURE_2D, 0);
}

void cgTextureDelete(cg_texture_t texture) {
  glDeleteTextures(1, &texture);
}
