#version 300 es
 precision mediump float;

 uniform mat4 viewProjection;

 layout (location = 0) in vec4 position;
 layout (location = 1) in vec3 offset;
 layout (location = 2) in float size;

 out vec4 color;

 void main() {
     color = vec4(1,1,1,1);
     gl_Position = viewProjection * vec4((position.xyz * size) + offset, position.w);
 }