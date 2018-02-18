#version 300 es
precision mediump float;

const float max = length(vec2(0.5, 0.5f));

in vec4 color;
in vec2 uv;

layout(location = 0) out vec4 color1;

void main() {
    float l =  length(uv) / max;
    color1 = color * (l > 0.5 ? 0.0f : 1.0f);
    //color1 = color * (l > 0.5 ? 0.0f : smoothstep(-max*0.5, max, l));
    //color1 = color * (1.0 - smoothstep(0.0, max, l));
    gl_FragDepth = color1.a > 0.0 ? gl_FragCoord.z : 1.0;
}