#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUV;

uniform mat4 uTransform;   
uniform mat4 uView;
uniform mat4 uProj;

out vec2 vUV;

void main() {
    vUV = aUV;
    gl_Position = uProj * uView * uTransform * vec4(aPos, 0.0, 1.0);
}