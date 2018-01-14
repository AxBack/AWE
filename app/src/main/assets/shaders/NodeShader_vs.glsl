#version 300 es
 precision mediump float;

 uniform mat4 viewProjection;
 uniform vec3 up;
 uniform vec3 right;

 layout (location = 0) in vec4 position;
 layout (location = 1) in vec3 offset;
 layout (location = 2) in float charge;

 out float lumocity;

 void main() {
     lumocity = charge;
     vec3 pos = right * position.x + up * position.y;
     gl_Position = viewProjection * vec4((pos * 1.0) + offset, position.w);
     //gl_Position = viewProjection * vec4((position.xyz * size) + offset, position.w);
 }