#version 300 es
 precision mediump float;

 in vec3 color;
 in float colorFactor;

 layout(location = 0) out vec4 color1;
 layout(location = 1) out vec4 color2;

 void main() {
     color1 = vec4(color, (1.0 - abs(colorFactor)));
     color2 = vec4(1,1,1,1);
 }