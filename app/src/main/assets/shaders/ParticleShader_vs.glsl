#version 300 es
 precision mediump float;

 uniform mat4 viewProjection;
 uniform vec3 up;
 uniform vec3 right;

 layout (location = 0) in vec4 position;
 layout (location = 1) in vec3 offset;
 layout (location = 2) in float size;
 layout (location = 3) in vec3 inColor;

 out vec4 color;

 void main() {
     color = vec4(inColor, 1);
     vec3 pos = right * position.x + up * position.y;
     gl_Position = viewProjection * vec4((pos * size) + offset, position.w);
     //gl_Position = viewProjection * vec4((position.xyz * size) + offset, position.w);
 }