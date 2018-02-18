#version 300 es
precision mediump float;

const float max = length(vec2(0.5, 0.5f));

in vec4 color;
in vec2 uv;

layout(location = 0) out vec4 color1;

void main() {
    color1 = color * (1.f - smoothstep(0.0, 0.5f, length(uv) / max));
}