#include "shader.h"

#include <stdlib.h>
#include <stdio.h>

#include "glcore.h"

static void f_cgShaderCompile(cg_shader_prog_t prog,
                              cg_shader_t shader,
                              const char *src) {
  glShaderSource(shader, 1, &src, NULL);
  glCompileShader(shader);

  { // checking compile status
    int compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
      int logLen;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);

      char log[logLen];
      glGetShaderInfoLog(shader, logLen, NULL, log);

      fprintf(stderr, "%s\n", log);
      exit(1);
    }
  } // checking compile status

  glAttachShader(prog, shader);
  glLinkProgram(prog);

  { // checking link status
    int linked;
    glGetProgramiv(prog, GL_LINK_STATUS, &linked);

    if (!linked) {
      int logLen;
      glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLen);

      char log[logLen];
      glGetProgramInfoLog(prog, logLen, NULL, log);

      fprintf(stderr, "%s\n", log);
      exit(1);
    }
  } // checking link status

  glDeleteShader(shader);
}

cg_shader_prog_t cgShaderProg(const char *vertSrc, const char *fragSrc) {
  cg_shader_prog_t prog = glCreateProgram();
  cg_shader_t vertShader = glCreateShader(GL_VERTEX_SHADER);
  cg_shader_t fragShader = glCreateShader(GL_FRAGMENT_SHADER);

  f_cgShaderCompile(prog, vertShader, vertSrc);
  f_cgShaderCompile(prog, fragShader, fragSrc);

  return prog;
}

void cgShaderProgBind(const cg_shader_prog_t prog) {
  glUseProgram(prog);
}

void cgShaderProgUnbind() {
  glUseProgram(0);
}

void cgShaderProgDelete(cg_shader_prog_t prog) {
  glDeleteProgram(prog);
}
