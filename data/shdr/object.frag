#version 330 core

#define MAX_MATERIALS 8
#define MAX_POINT_LIGHTS 128
#define LIGHT_BRIGHTNESS_INTENSITY 1.5f

struct Material {
  vec4 albedoCol, brightCol;
  int albedoTexEnabled, brightTexEnabled;
  sampler2D albedoTex, brightTex;
};

struct PointLight {
  vec3 pos;
  vec3 col;
  float radius;
};

flat in uint f_index, f_materialIndex;
in vec3 f_pos, f_norm;
in vec2 f_uv;

uniform float u_time;
uniform mat4 u_projMat, u_viewMat, u_transMat;
uniform vec4 u_ambientCol;
uniform Material u_materials[MAX_MATERIALS];
uniform PointLight u_pointLights[MAX_POINT_LIGHTS];

layout (location = 0) out vec4 o_fragCol;
layout (location = 1) out vec4 o_brightCol;

vec3 diffuseLight() {
  vec3 col;

  for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
    if (u_pointLights[i].col == vec3(0.0f))
      continue;

    vec3 lightPos = u_pointLights[i].pos;
    vec3 fragPos = vec3(u_transMat * vec4(f_pos, 1.0f));

    vec3 lightCol = u_pointLights[i].col;
    float lightRadius = u_pointLights[i].radius;

    vec3 norm = mat3(u_transMat) * normalize(f_norm);
    vec3 direction = lightPos - fragPos;

    float amount = dot(normalize(norm), normalize(direction));
    amount = max(amount, 0.0f);

    float dist = distance(fragPos, lightPos);
    float distanceFactor = clamp(1.0f - dist * dist
                                 / (lightRadius * lightRadius),
                                 0.0f, 1.0f);
    amount *= distanceFactor;

    col += vec3(lightCol.r * amount,
                lightCol.g * amount,
                lightCol.b * amount);
  }

  return col;
}

void main() {
  o_fragCol = u_materials[f_materialIndex].albedoCol;
  o_brightCol = u_materials[f_materialIndex].brightCol;

  if (u_materials[f_materialIndex].albedoTexEnabled == 1)
    o_fragCol *= texture(u_materials[f_materialIndex].albedoTex, f_uv);

  if (u_materials[f_materialIndex].brightTexEnabled == 1)
    o_brightCol *= texture(u_materials[f_materialIndex].brightTex, f_uv);

  o_fragCol *= u_ambientCol + vec4(diffuseLight(), 1.0f);
  o_brightCol += u_ambientCol;
  o_brightCol.rgb += diffuseLight();
  o_brightCol.rgb -= vec3(1.0f);
  o_brightCol.rgb *= LIGHT_BRIGHTNESS_INTENSITY;
}
