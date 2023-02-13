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
void Cube::configureTextures()
{

}

void Cube::addTexture(char *texture_path)
{

}

void Cube::setColor(glm::vec3 colorValue, float brightnessNormalized, int speed)
{

}

void Cube::orbit(int radius, float angle, glm::vec3 origin, glm::vec3 axis_of_orbit)
{

}

void Cube::setTransformMatrix(glm::mat4& matrix, float angle, glm::vec3 vector_translate, glm::vec3 vector_rotate, glm::vec3 vector_scale)
{

}