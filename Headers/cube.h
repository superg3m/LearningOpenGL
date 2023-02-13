#ifndef CUBE_H
#define CUBE_H

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Headers/shader.h>

class Cube
{
public:
	void init(Shader &shader, GLfloat *vertices, const int &arrayLength, const int& size_in_bits, unsigned int &cube_VBO, unsigned int &cube_VAO);

	Shader shader;
	GLfloat *vertices;

	int arrayLength;
	int size_in_bits;

	unsigned int cube_VBO;
	unsigned int cube_VAO;



	
private:

};




#endif