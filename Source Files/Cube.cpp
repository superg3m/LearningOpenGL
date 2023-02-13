#include <Headers/cube.h>


// Why can't this be a constructor?
void Cube::init(Shader& shader, GLfloat* vertices, const int& arrayLength, const int& size_in_bits, unsigned int& cube_VBO, unsigned int& cube_VAO)
{
	this->shader = shader;
	this->vertices = vertices;
	this->arrayLength = arrayLength;
	this->size_in_bits = size_in_bits;
	this->cube_VBO = cube_VBO;
	this->cube_VAO = cube_VAO;
}