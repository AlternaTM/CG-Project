#version 330 core

in vec2 vUV;

out vec4 FragColor;

uniform vec4 uColor;

uniform float uTime;

void main()
{

    float dist = abs(vUV.y - 0.5);

    float core = 0.15;
    float glow = 0.25;



    float alpha = 1.0 - smoothstep(core, core + glow, dist);

    float hihihi = smoothstep(0.0, 0.05, vUV.x) *(1.0 - smoothstep(0.95, 1.0, vUV.x));


    float pulse = 0.75 + 0.25 * sin(uTime * 12.0);
    
    float noise = sin(vUV.x * 30.0 + uTime * 20.0) * 0.05;
    alpha += noise;

    alpha *= hihihi * pulse;

    FragColor = vec4(uColor.rgb, uColor.a * alpha * pulse);
}