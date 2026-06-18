#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform vec3 viewPos;

void main()
{


    vec3 ambient = vec3(0.8, 0.8, 0.8);
    vec3 result = ambient * texture(texture_diffuse1, TexCoords).rgb;

    FragColor = vec4(result, 1.0);
    //FragColor = vec4(norm * 0.5 + 0.5, 1.0); 
   //FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
