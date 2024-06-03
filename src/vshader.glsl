#version 410

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec3 vNormal;
layout(location = 3) in vec2 vTexCoord;

uniform mat4 ModelView;
uniform mat4 Projection;

out vec4 color;
out vec3 Normal;
out vec4 Position;
out vec2 TexCoord;

void main() {
    color = vColor;
    Normal = normalize(vNormal);
    Position = ModelView * vPosition;
    TexCoord = vTexCoord;
    gl_Position = Projection * Position;
}