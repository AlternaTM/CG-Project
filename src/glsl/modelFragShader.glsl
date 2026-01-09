#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform vec3 lightPos1;
uniform vec3 lightColor1;

uniform vec3 lightPos2;
uniform vec3 lightColor2;

uniform vec3 viewPos;

void main()
{
    vec3 diffuseTex  = texture(texture_diffuse1, TexCoords).rgb;
    vec3 specularTex = texture(texture_specular1, TexCoords).rgb;

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0);

    // ---- Luce 1 ----
    {
        vec3 lightDir = normalize(lightPos1 - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

        vec3 ambient  = 0.05 * diffuseTex;
        vec3 diffuse  = diff * diffuseTex * lightColor1;
        vec3 specular = spec * specularTex * lightColor1;

        result += ambient + diffuse + specular;
    }

    // ---- Luce 2 ----
    {
        vec3 lightDir = normalize(lightPos2 - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

        vec3 ambient  = 0.03 * diffuseTex;
        vec3 diffuse  = diff * diffuseTex * lightColor2;
        vec3 specular = spec * specularTex * lightColor2;

        result += ambient + diffuse + specular;
    }

    FragColor = vec4(result, 1.0);
}
