#version 300 es
 precision mediump float;

 in vec3 color;
 in vec2 uv;
 in float charge;

 layout(location = 0) out vec4 color1;
 layout(location = 1) out vec4 color2;

 void main() {
     color1 =  vec4(color, 1) * (1.0- smoothstep(0.0, 0.7, length(vec2(0.5,0.5f)-uv)));
     color2 = color1 * charge;
 }