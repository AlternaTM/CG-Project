#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUV;   

out vec2 vUV;

uniform mat4 uTransform;
uniform mat4 uView;       
uniform mat4 uProj;

void main() {
    vUV = aUV;
    gl_Position = uView * uProj * uTransform * vec4(aPos, 0.0, 1.0);
}