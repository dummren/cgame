#ifndef __CG__TEXTURE_H__
#define __CG__TEXTURE_H__

typedef unsigned int cg_texture_t;

cg_texture_t cgTextureEmpty(const unsigned int,
                            const int, const int,
                            const int, const int,
                            const int, const int,
                            const int, const int);

cg_texture_t cgTextureLoad(const unsigned char*,
                           const unsigned int,
                           const int, const int,
                           const int, const int,
                           const int, const int,
                           const int, const int);

cg_texture_t cgTextureLoadFromMemory(const unsigned char*,
                                     const unsigned int,
                                     const int,
                                     int*, int*,
                                     int*,
                                     const int,
                                     const int,
                                     const int,
                                     const int, const int,
                                     const int, const int);

void cgTextureBind(const cg_texture_t);
void cgTextureUnbind();

void cgTextureDelete(cg_texture_t);

#endif // __CG__TEXTURE_H__
