#version 300 es
precision mediump float;

uniform mat4 viewProjection;

layout (location = 0) in vec4 position;
layout (location = 1) in float tensionImpact;
layout (location = 2) in vec3 start;
layout (location = 3) in vec3 end;
layout (location = 4) in float tension;
layout (location = 5) in vec3 startColor;
layout (location = 6) in vec3 endColor;

out vec4 color;

void main() {
    color = vec4(startColor * (1.0 - position.y) + endColor * position.y,1);

    vec3 right = cross(normalize(end-start), vec3(0,0,-1));
    vec3 pos = (start * (1.0 - position.y) + end * position.y)
             + right * (position.x + (tensionImpact * (position.x * tension)));

    gl_Position = viewProjection * vec4(pos, position.w);
}