#version 300 es
 precision mediump float;

 uniform mat4 viewProjection;
 uniform vec3 up;
 uniform vec3 right;
 uniform vec3 forward;

 layout (location = 0) in vec4 position;
 layout (location = 1) in vec3 offset;
 layout (location = 2) in vec3 baseColor;
 layout (location = 3) in float size;
 layout (location = 4) in float charge;

 out vec4 color;
 out vec2 uv;
 out float intensity;

 void main() {
     color = vec4(baseColor, 1.0f);
     uv = position.xy;
     intensity = 1.0f + charge;
     vec3 pos = ((right * position.x + up * position.y ) * (2.0 + charge));
     gl_Position = viewProjection * vec4((pos * size) + offset, position.w);
 }