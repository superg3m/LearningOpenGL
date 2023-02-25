#version 330 core
out vec4 FragColor;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 ourColor;

uniform vec4 uniColor;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    

    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.color * light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.color * light.specular * spec * texture(material.specular, TexCoords).rgb;  
        
    vec3 result = ambient + diffuse + specular; 
     
    FragColor = vec4(ourColor, 1.0) * vec4(result, 1.0);
    //FragColor = texture(ourTexture1, TexCoords) + texture(ourTexture2, TexCoords) * vec4(result, 1.0);
    FragColor += uniColor;
}