#version 330 core
out vec4 FragColor;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 TexCoord;

in vec3 FragPos;
in vec3 Normal;


in vec4 ourColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec4 uniColor;



uniform Material material;
uniform Light light;

uniform sampler2D ourTexture;

void main()
{
    

    // ambient
    float ambientStrength = 0.5f;
    vec3 ambient = light.ambient * material.ambient * ambientStrength;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * (diff * material.diffuse);
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lightColor * (spec * material.specular); 
    
    
    vec3 result = (ambient + diffuse + specular);
    FragColor = texture(ourTexture, TexCoord) * ourColor * vec4(result, 1.0);
    FragColor += uniColor;
}