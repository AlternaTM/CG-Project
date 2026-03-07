#version 330 core

out vec4 FragColor;

in vec2 vTex;

uniform sampler2D uTexture;
uniform vec3 uColor; // tint opzionale

void main()
{
    vec4 tex = texture(uTexture, vTex);
    FragColor = tex * vec4(uColor, 1.0);
}
