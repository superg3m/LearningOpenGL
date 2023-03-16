#version 330 core
out vec4 FragColor;

struct Material 
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
    float type;
}; 

struct DirLight 
{
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct LightNode 
{
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 attenuationCoefficients;
};
struct SpotLight 
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;      
    
    float flashLightMode;
};

#define NR_POINT_LIGHTS 10

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(LightNode light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 ourColor;

uniform vec3 viewPos;
uniform vec3 lightColor;
uniform DirLight dirLight;
uniform LightNode lightNode[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;
uniform vec4 uniColor;

void main()
{
    

    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // phase 2: point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        result += CalcPointLight(lightNode[i], norm, FragPos, viewDir);  
    }
    // phase 3: spot light
    if(spotLight.flashLightMode == 1)
    {
        result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    }

    if(material.type == 1)
    {
        FragColor = uniColor; // Change this for texture
    }
    else
    {
        FragColor = vec4(ourColor, 1.0) * vec4(result, 1.0); // Change this for texture
    } 
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = lightColor * light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse = lightColor * light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = lightColor * light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(LightNode light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);

    float attenuation = 0;
    if(light.constant > 0 || light.linear > 0 || light.quadratic > 0) {
        attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    }  
    // combine results
    vec3 ambient = lightColor * light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse = lightColor * light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = lightColor * light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);

    float attenuation = 0;
    if(light.constant > 0 || light.linear > 0 || light.quadratic > 0) {
        attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    }  
    
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return  (ambient + diffuse + specular);
}