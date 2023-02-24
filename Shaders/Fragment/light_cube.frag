#version 330 core
out vec4 FragColor;
uniform vec3 uniColor2;

void main()
{

    FragColor = vec4(uniColor2, 1.0); // set all 4 vector values to 1.0

}