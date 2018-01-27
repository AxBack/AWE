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
layout (location = 5) in float rand;

out vec4 color;

void main() {
    color = vec4(1.0,1.0,1.0,1.0);

    vec3 pos = mix(start, end, dt);

    vec3 toCamera = normalize(cameraPos - pos);
    vec3 up = cross(toCamera, normalize(end - start));

    pos += up * position.y + (influence * normalize(texture(uTexture, vec2(dt, rand)).xyz - 0.5f) * 1.0f );

    gl_Position = viewProjection * vec4(pos, position.w);
}