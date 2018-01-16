#version 300 es
 precision mediump float;

 layout (location = 0) in vec4 position;
 layout (location = 1) in vec2 texCoord;

 out vec2 uv;

 void main() {
    uv = texCoord;
    gl_Position = position;
 }