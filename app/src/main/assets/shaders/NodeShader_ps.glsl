#version 300 es
precision mediump float;

in float lumocity;

out vec4 finalColor;

void main() {
    finalColor = mix(vec4(0,0,1,1), vec4(1,0,0,1), lumocity);
}