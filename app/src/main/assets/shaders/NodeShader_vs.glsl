#version 300 es
 precision mediump float;

 uniform mat4 viewProjection;
 uniform vec3 up;
 uniform vec3 right;

 layout (location = 0) in vec4 position;
 layout (location = 1) in vec3 offset;
 layout (location = 2) in float charge;
 layout (location = 3) in vec3 baseColor;

 out vec4 color;
 out vec2 uv;

 void main() {
     color = mix(vec4(baseColor, 1), vec4(1,1,1,1), charge);
     uv = position.xy;
     vec3 pos = right * position.x + up * position.y;
     gl_Position = viewProjection * vec4((pos * 1.0) + offset, position.w);
 }