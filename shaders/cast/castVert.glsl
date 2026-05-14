#version 330 core

layout(location = 0) in vec2 aPos;

out vec2 vUV;

uniform vec2 uPosition;
uniform vec2 uSize;
uniform float uAngle;

uniform mat4 uView;
uniform mat4 uProj;

void main()
{

    vUV = aPos;
    vec2 scaled = (aPos -0.5) * uSize;

    vec2 rotated = vec2(
        scaled.x * cos(uAngle) - scaled.y * sin(uAngle),
        scaled.x * sin(uAngle) + scaled.y * cos(uAngle)
    );

    vec2 forward = vec2(cos(uAngle), sin(uAngle));

    vec2 worldPos = uPosition  + rotated + forward * (uSize.x * 0.5);

    gl_Position = uProj * uView * vec4(worldPos, 0.0, 1.0);
}