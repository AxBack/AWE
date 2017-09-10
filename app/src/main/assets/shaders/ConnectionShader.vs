#version 300 es
precision mediump float;

uniform mat4 viewProjection;

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 start;
layout (location = 2) in vec3 end;
layout (location = 3) in float tension;

out vec4 color;

void main() {
    color = vec4(1,1,1,1);

    vec3 right = cross(normalize(end-start), vec3(0,0,-1));
    vec3 pos = (start * (1.0 - position.y) + end * position.y) + right * position.x * (1.0 + tension);

    gl_Position = viewProjection * vec4(pos, position.w);
}