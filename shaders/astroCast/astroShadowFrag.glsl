#version 330 core

in vec2 vUV;

out vec4 FragColor;

uniform vec4 uColor;
uniform float uTime;

void main()
{

    vec2 centered = vUV - vec2(0.5);
    float dist = length(centered);


    float progrss = clamp(uTime,0.0,1.0);

    float core =  mix(0.4, 0.20, progrss);
    float glow = 0.15;
    float alpha = 1.0 - smoothstep(core, core + glow, dist);


    FragColor = vec4(uColor.rgb, uColor.a * alpha);
}