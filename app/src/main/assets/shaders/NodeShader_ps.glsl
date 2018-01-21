#version 300 es
precision mediump float;

const float max = length(vec2(0.5, 0.5f));

in vec4 color;
in vec2 uv;
in float intensity;

layout(location = 0) out vec4 color1;
layout(location = 1) out vec4 color2;

void main() {
    float l =  (length(uv) / max);
    color1 = color * (l > 0.25 ? 0.0f : 1.0f);
    float i = 0.30 + (0.2 * intensity);
    color2.a = (l > i ? 0.0f : 1.0f);
    color2.rgb = mix(vec3(1,1,1), color.rgb, l / i);
    gl_FragDepth = color2.a > 0.0 ? gl_FragCoord.z : 1.0;
}