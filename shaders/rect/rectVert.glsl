#version 330 core

layout(location = 0) in vec2 aPos;

uniform vec2 uPosition;
uniform vec2 uSize;
uniform mat4 uView;
uniform mat4 uProj;

void main()
{
    vec2 worldPos = uPosition + aPos * uSize;
    gl_Position = uProj* uView * vec4(worldPos, 0.0, 1.0);
}