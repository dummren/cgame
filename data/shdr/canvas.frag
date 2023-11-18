#version 330 core

#define BLUR_STEPS 16
#define BLUR_INTENSITY 0.025f

in vec3 f_pos;
in vec2 f_uv;

uniform int u_bloomEnabled;
uniform sampler2D u_fragTex;
uniform sampler2D u_brightTex;

out vec4 o_fragCol;

vec4 bloomTex() {
  vec4 brightTex = texture(u_brightTex, f_uv);

  vec3 tex = brightTex.rgb;
  vec2 texelSize = 1.0f / textureSize(u_brightTex, 0);

  for (int i = 0; i < BLUR_STEPS; i++) {
    tex += texture(u_brightTex, f_uv + vec2(texelSize.x * i, 0.0f)).rgb;
    tex += texture(u_brightTex, f_uv - vec2(texelSize.x * i, 0.0f)).rgb;

    tex += texture(u_brightTex, f_uv + vec2(0.0f, texelSize.y * i)).rgb;
    tex += texture(u_brightTex, f_uv - vec2(0.0f, texelSize.y * i)).rgb;
  }

  return vec4(tex * BLUR_INTENSITY, 1.0f);
}

void main() {
  o_fragCol = texture(u_fragTex, f_uv);

  if (u_bloomEnabled == 1)
    o_fragCol += bloomTex();
}
