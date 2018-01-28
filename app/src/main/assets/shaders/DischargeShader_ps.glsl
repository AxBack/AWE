#version 300 es
 precision mediump float;

 in vec4 color;

 layout(location = 0) out vec4 color1;
 layout(location = 1) out vec4 color2;

 void main() {
     color1 = color;
     color2 = vec4(1,1,1,1);
 }