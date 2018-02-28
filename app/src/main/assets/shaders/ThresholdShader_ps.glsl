#version 300 es
precision mediump float;

uniform sampler2D uTexture;

in vec2 uv;

out vec4 finalColor;

const float magnitude = 1.0f;

void main() {
    finalColor = texture(uTexture, uv);
    finalColor *= length(finalColor.xyz) < 0.1f ? 0.0f : 1.0f;
    //finalColor = mix(vec4(0,0,0,0), finalColor, length(finalColor.xyz) / 1.0f);
}