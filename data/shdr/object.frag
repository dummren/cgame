#version 330 core

#define MAX_MATERIALS 8

struct Material {
  vec4 albedoCol, brightCol;
  int albedoTexEnabled, brightTexEnabled;
  sampler2D albedoTex, brightTex;
};

flat in uint f_index, f_materialIndex;
in vec3 f_pos, f_norm;
in vec2 f_uv;

uniform float u_time;
uniform Material u_materials[MAX_MATERIALS];

layout (location = 0) out vec4 o_fragCol;
layout (location = 1) out vec4 o_brightCol;

void main() {
  o_fragCol = u_materials[f_materialIndex].albedoCol;
  o_brightCol = u_materials[f_materialIndex].brightCol;

  if (u_materials[f_materialIndex].albedoTexEnabled == 1)
    o_fragCol *= texture(u_materials[f_materialIndex].albedoTex, f_uv);

  if (u_materials[f_materialIndex].brightTexEnabled == 1)
    o_brightCol *= texture(u_materials[f_materialIndex].brightTex, f_uv);
}
