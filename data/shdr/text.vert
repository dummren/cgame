#version 330 core

layout (location = 2) in vec3 v_pos;
layout (location = 3) in vec2 v_uv;

uniform mat4 u_projMat, u_transMat;
uniform vec2 u_size;

out vec3 f_pos;
out vec2 f_uv;

void main() {
  f_pos = v_pos;
  f_uv = v_uv;

  vec3 pos = v_pos * vec3(u_size, 1.0f);
  gl_Position = u_projMat * u_transMat * vec4(pos, 1.0f);
}
