#version 300 es
precision mediump float;

uniform mat4 viewProjection;
uniform sampler2D uTexture;
uniform vec3 up;
uniform vec3 right;

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 offset;
layout (location = 3) in vec3 inColor;
layout (location = 4) in float size;

out vec3 color;
out vec2 uv;
out float charge;

void main() {
    color = inColor;
    uv = texCoords;
    charge = size;

    vec3 pos = offset + (right * position.x + up * position.y) * size;

    gl_Position = viewProjection * vec4(pos, position.w);
}