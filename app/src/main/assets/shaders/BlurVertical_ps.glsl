#version 300 es
precision mediump float;

uniform sampler2D uTexture;

in vec2 uv;

out vec4 finalColor;

const float weights[] = float[](0.000229,0.005977,0.060598,0.241732,0.382928,0.241732,0.060598,0.005977,0.000229);

void main() {
    finalColor = vec4(0,0,0,0);

    finalColor += textureOffset(uTexture, uv, ivec2(0,-4)) * weights[0];
    finalColor += textureOffset(uTexture, uv, ivec2(0,-3)) * weights[1];
    finalColor += textureOffset(uTexture, uv, ivec2(0,-2)) * weights[2];
    finalColor += textureOffset(uTexture, uv, ivec2(0,-1)) * weights[3];
    finalColor += textureOffset(uTexture, uv, ivec2(0, 0)) * weights[4];
    finalColor += textureOffset(uTexture, uv, ivec2(0, 1)) * weights[5];
    finalColor += textureOffset(uTexture, uv, ivec2(0, 2)) * weights[6];
    finalColor += textureOffset(uTexture, uv, ivec2(0, 3)) * weights[7];
    finalColor += textureOffset(uTexture, uv, ivec2(0, 4)) * weights[8];

}