#version 330 core

layout (location = 1) in vec3 v_pos;
layout (location = 2) in vec2 v_uv;

out vec3 f_pos;
out vec2 f_uv;

void main() {
  f_pos = v_pos;
  f_uv = v_uv;

  gl_Position = vec4(v_pos, 1.0f);
}
