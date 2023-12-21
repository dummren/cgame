#include "glcore.h"

#include <GL/gl.h>
#include <GL/glext.h>

#ifdef __linux__
#include <GL/glx.h>
#include <GL/glxext.h>
#endif // __linux__

// declaration of opengl functions (without extern keyword this time)
GLCORE_DEFINES();

static void f_load() {
#define STR(x) #x

#ifdef __linux__
#define LOAD(x, t) x = (t) glXGetProcAddressARB(STR(x))
#elif __WIN32__ // __linux__
#define LOAD(x, t) x = (t) wglGetProcAddress(STR(x))
#endif // __WIN32__

  LOAD(glGenFramebuffers, PFNGLGENFRAMEBUFFERSPROC);
  LOAD(glBindFramebuffer, PFNGLBINDFRAMEBUFFERPROC);
  LOAD(glGenRenderbuffers, PFNGLGENRENDERBUFFERSPROC);
  LOAD(glBindRenderbuffer, PFNGLBINDRENDERBUFFERPROC);
  LOAD(glFramebufferTexture2D, PFNGLFRAMEBUFFERTEXTURE2DPROC);
  LOAD(glRenderbufferStorage, PFNGLRENDERBUFFERSTORAGEPROC);
  LOAD(glFramebufferRenderbuffer, PFNGLFRAMEBUFFERRENDERBUFFERPROC);
  LOAD(glDrawBuffers, PFNGLDRAWBUFFERSPROC);
  LOAD(glCreateProgram, PFNGLCREATEPROGRAMPROC);
  LOAD(glDeleteProgram, PFNGLDELETEPROGRAMPROC);
  LOAD(glCreateShader, PFNGLCREATESHADERPROC);
  LOAD(glDeleteShader, PFNGLDELETESHADERPROC);
  LOAD(glShaderSource, PFNGLSHADERSOURCEPROC);
  LOAD(glCompileShader, PFNGLCOMPILESHADERPROC);
  LOAD(glGetShaderiv, PFNGLGETSHADERIVPROC);
  LOAD(glGetShaderInfoLog, PFNGLGETSHADERINFOLOGPROC);
  LOAD(glAttachShader, PFNGLATTACHSHADERPROC);
  LOAD(glLinkProgram, PFNGLLINKPROGRAMPROC);
  LOAD(glGetProgramiv, PFNGLGETPROGRAMIVPROC);
  LOAD(glGetProgramInfoLog, PFNGLGETPROGRAMINFOLOGPROC);
#ifdef __WIN32__
  LOAD(glActiveTexture, PFNGLACTIVETEXTUREPROC);
#endif // __WIN32__
  LOAD(glDeleteFramebuffers, PFNGLDELETEFRAMEBUFFERSPROC);
  LOAD(glDeleteRenderbuffers, PFNGLDELETERENDERBUFFERSPROC);
  LOAD(glGenVertexArrays, PFNGLGENVERTEXARRAYSPROC);
  LOAD(glBindVertexArray, PFNGLBINDVERTEXARRAYPROC);
  LOAD(glDeleteVertexArrays, PFNGLDELETEVERTEXARRAYSPROC);
  LOAD(glGenBuffers, PFNGLGENBUFFERSPROC);
  LOAD(glBindBuffer, PFNGLBINDBUFFERPROC);
  LOAD(glBufferData, PFNGLBUFFERDATAPROC);
  LOAD(glEnableVertexAttribArray, PFNGLENABLEVERTEXATTRIBARRAYPROC);
  LOAD(glVertexAttribPointer, PFNGLVERTEXATTRIBPOINTERPROC);
  LOAD(glVertexAttribIPointer, PFNGLVERTEXATTRIBIPOINTERPROC);
  LOAD(glDeleteBuffers, PFNGLDELETEBUFFERSPROC);
  LOAD(glUniform1i, PFNGLUNIFORM1IPROC);
  LOAD(glUniform1f, PFNGLUNIFORM1FPROC);
  LOAD(glUniform2f, PFNGLUNIFORM2FPROC);
  LOAD(glUniform3fv, PFNGLUNIFORM3FVPROC);
  LOAD(glUniform4fv, PFNGLUNIFORM4FVPROC);
  LOAD(glUniformMatrix4fv, PFNGLUNIFORMMATRIX4FVPROC);
  LOAD(glUseProgram, PFNGLUSEPROGRAMPROC);
  LOAD(glGetUniformLocation, PFNGLGETUNIFORMLOCATIONPROC);
  LOAD(glGenerateMipmap, PFNGLGENERATEMIPMAPPROC);

#undef STR
#undef LOAD
}

void initGlFunctions() {
  f_load();
}
