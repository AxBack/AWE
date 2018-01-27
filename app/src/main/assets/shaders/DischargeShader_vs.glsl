#version 300 es
precision mediump float;

uniform mat4 viewProjection;
uniform sampler2D uTexture;

layout (location = 0) in vec4 position;
layout (location = 1) in float dt;
layout (location = 2) in vec3 start;
layout (location = 3) in vec3 end;
layout (location = 4) in float rand;

out vec4 color;

void main() {
    color = vec4(1.0,1.0,1.0,1.0);

    vec3 right = normalize(end - start);
    vec3 up = cross(right, vec3(0,0,1));
    vec3 pos = mix(start, end, dt);
    pos += up * position.y * (texture(uTexture, vec2(dt, rand)).xyz - 0.5f) * 5.0f;

    gl_Position = viewProjection * vec4(pos, position.w);
}