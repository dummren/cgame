#version 330 core

layout (location = 0) in uint v_index;
layout (location = 1) in uint v_materialIndex;
layout (location = 2) in vec3 v_pos;
layout (location = 3) in vec3 v_norm;
layout (location = 4) in vec2 v_uv;

uniform float u_time;
uniform mat4 u_projMat, u_viewMat, u_transMat;

flat out uint f_index, f_materialIndex;
out vec3 f_pos, f_norm;
out vec2 f_uv;

void main() {
  f_index = v_index;
  f_materialIndex = v_materialIndex;
  f_pos = v_pos;
  f_norm = v_norm;
  f_uv = v_uv;

  gl_Position = u_projMat * u_viewMat * u_transMat * vec4(v_pos, 1.0f);
}
