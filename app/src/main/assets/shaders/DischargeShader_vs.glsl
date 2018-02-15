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

out vec3 color;
out float colorFactor;

const float size = 0.3f;

void main() {
    color = inColor * (1.0f + (rand * 0.75f));
    colorFactor = position.y;

    float l = min(length(end - start) / 10.0f, 1.0f);
    vec3 pos = mix(start, end, dt);
    vec3 toCamera = normalize(cameraPos - pos);
    vec3 toEnd = normalize(end - start);
    vec3 up = normalize(cross(toCamera, toEnd));

    vec3 offset = texture(uTexture, vec2(dt, rand)).xyz - 0.5f;
    offset = (up * (size * position.y) + (l * influence * normalize(offset)));
    pos += offset;

    gl_Position = viewProjection * vec4(pos, position.w);
}