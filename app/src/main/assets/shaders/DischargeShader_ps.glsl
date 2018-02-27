#version 300 es
 precision mediump float;

const float max = length(vec2(0.25, 0.25f));

 in vec3 color;
 in vec2 uv;
 in float charge;

 out vec4 color1;

 void main() {
     float l = 1.0f - (length(vec2(0.5, 0.5f) - uv) / max);
     //color = color * (l > 0.5 ? 0.0f : 1.0f) * charge;
     color1 = vec4(color, 1) * l * charge;
 }