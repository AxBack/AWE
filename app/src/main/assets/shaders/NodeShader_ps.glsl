#version 300 es
precision mediump float;

const float max = length(vec2(0.5, 0.5f));

in vec4 color;
in vec2 uv;

out vec4 finalColor;

void main() {
    float l =  (length(uv) / max) > 0.5 ? 0.0 : 1.0;
    finalColor = color * l;
    gl_FragDepth = l > 0.0 ? gl_FragCoord.z : 1.0;
}