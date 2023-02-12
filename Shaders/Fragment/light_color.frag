#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

in vec4 ourColor;
uniform vec4 uniColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    

    //Ambient light
    float ambientStrength = 1.0;
    vec3 ambient = ambientStrength * lightColor;

    // Diffused lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightColor), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular Lighting
    float specularStrength = 10.5f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = specularStrength * spec * lightColor;
    
    
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = texture(ourTexture, TexCoord) * ourColor * vec4(result, 1.0);
    FragColor += uniColor;
}