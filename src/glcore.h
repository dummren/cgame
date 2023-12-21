#ifndef __CG__GLCORE_H__
#define __CG__GLCORE_H__

#include <GL/gl.h>
#include <GL/glext.h>

#ifdef __linux__
#include <GL/glx.h>
#include <GL/glxext.h>
#endif // __linux__

#include "GLFW/glfw3.h"

#define _GLCORE_DEFINES(x)                                       \
  x PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;                  \
  x PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;                  \
  x PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;                \
  x PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;                \
  x PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;        \
  x PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;          \
  x PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;  \
  x PFNGLDRAWBUFFERSPROC glDrawBuffers;                          \
  x PFNGLCREATEPROGRAMPROC glCreateProgram;                      \
  x PFNGLDELETEPROGRAMPROC glDeleteProgram;                      \
  x PFNGLCREATESHADERPROC glCreateShader;                        \
  x PFNGLDELETESHADERPROC glDeleteShader;                        \
  x PFNGLSHADERSOURCEPROC glShaderSource;                        \
  x PFNGLCOMPILESHADERPROC glCompileShader;                      \
  x PFNGLGETSHADERIVPROC glGetShaderiv;                          \
  x PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;                \
  x PFNGLATTACHSHADERPROC glAttachShader;                        \
  x PFNGLLINKPROGRAMPROC glLinkProgram;                          \
  x PFNGLGETPROGRAMIVPROC glGetProgramiv;                        \
  x PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;              \
  x PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;            \
  x PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;          \
  x PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;                  \
  x PFNGLBINDVERTEXARRAYPROC glBindVertexArray;                  \
  x PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;            \
  x PFNGLGENBUFFERSPROC glGenBuffers;                            \
  x PFNGLBINDBUFFERPROC glBindBuffer;                            \
  x PFNGLBUFFERDATAPROC glBufferData;                            \
  x PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;  \
  x PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;          \
  x PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer;        \
  x PFNGLDELETEBUFFERSPROC glDeleteBuffers;                      \
  x PFNGLUNIFORM1IPROC glUniform1i;                              \
  x PFNGLUNIFORM1FPROC glUniform1f;                              \
  x PFNGLUNIFORM2FPROC glUniform2f;                              \
  x PFNGLUNIFORM3FVPROC glUniform3fv;                            \
  x PFNGLUNIFORM4FVPROC glUniform4fv;                            \
  x PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;                \
  x PFNGLUSEPROGRAMPROC glUseProgram;                            \
  x PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;            \
  x PFNGLGENERATEMIPMAPPROC glGenerateMipmap

#define GLCORE_DEFINES(x) _GLCORE_DEFINES(x)

// for some reason linux thinks that glActiveTexture is opengl 1.1 function
// but in reality it is supported by 1.3 opengl version
#ifdef __WIN32__
#define GLCORE_DEFINES(x) _GLCORE_DEFINES(x); \
  x PFNGLACTIVETEXTUREPROC glActiveTexture
#endif // __WIN32__

// extern forward declaration of all functions (they will be defined in
// glcore.c)
GLCORE_DEFINES(extern);

void initGlFunctions();

#endif // __CG__GLCORE_H__
