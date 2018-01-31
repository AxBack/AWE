#version 300 es
precision mediump float;

uniform mat4 viewProjection;
uniform sampler2D uTexture;
uniform vec3 cameraPos;

layout (location = 0) in vec4 position;
layout (location = 1) in float influence;
layout (location = 2) in float dt;
layout (location = 3) in vec3 start;
layout (location = 4) in vec3 end;
layout (location = 5) in vec3 inColor;
layout (location = 6) in float rand;

out vec4 color;

const float size = 0.3f;

void main() {
    color = vec4(inColor, 1.0f) * (1.0f + (rand * 0.75f));

    vec3 pos = mix(start, end, dt);
    vec3 up = normalize(cross(normalize(cameraPos - pos), normalize(end - start)));

    vec3 offset = (influence * normalize(texture(uTexture, vec2(dt, rand)).xyz - 0.5f) * 1.0f );

    pos += (up * (size * position.y) + offset) * length(up);

    gl_Position = viewProjection * vec4(pos, position.w);
}