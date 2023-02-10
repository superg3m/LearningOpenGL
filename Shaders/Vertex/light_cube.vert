#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 transform;

void main()
{
	FragPos = vec3(model * vec4(aPos, 1.0f));
	Normal = mat3(transpose(inverse(model))) * aNormal;  
	if(transform == 0)  gl_Position = projection * view * model * vec4(FragPos, 1.0);
    else 
    {
        gl_Position = projection * view * model * transform * vec4(FragPos, 1.0);
    }
}