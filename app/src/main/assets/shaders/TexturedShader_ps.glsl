#version 300 es
 precision mediump float;

uniform sampler2D uTexture;

 in vec2 uv;

 out vec4 finalColor;

 void main() {
     finalColor = texture(uTexture, uv);
 }