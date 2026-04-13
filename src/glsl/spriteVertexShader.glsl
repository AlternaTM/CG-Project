#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

out vec2 vTex;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

uniform vec2 uUVOffset;
uniform vec2 uUVSize;

void main()
{
    vTex = aTex * uUVSize + uUVOffset;
    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
}