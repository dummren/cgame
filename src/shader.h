#ifndef __CG__SHADER_H__
#define __CG__SHADER_H__

typedef unsigned int cg_shader_t;
typedef unsigned int cg_shader_prog_t;

cg_shader_prog_t cgShaderProg(const char*, const char*);

void cgShaderProgBind(const cg_shader_prog_t);
void cgShaderProgUnbind();

void cgShaderProgDelete(cg_shader_prog_t);

#endif // __CG__SHADER_H__
