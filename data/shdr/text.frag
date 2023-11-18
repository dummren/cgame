#version 330 core

in vec3 f_pos;
in vec2 f_uv;

uniform sampler2D u_tex;
uniform vec4 u_color;

layout (location = 0) out vec4 o_fragCol;
layout (location = 1) out vec4 o_brightCol;

void main() {
  o_fragCol = vec4(1.0f, 1.0f, 1.0f, texture(u_tex, f_uv));
  o_fragCol *= u_color;

  if (o_fragCol.a < 0.5f)
    discard;

  o_brightCol = vec4(o_fragCol.rgb * 0.3f, 1.0f);
}
