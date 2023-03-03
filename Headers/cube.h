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

	void configureTextures();
	void addTexture(char* texturePath);
	void setColor(glm::vec3 colorValue, float brightnessNormalized, int speed);
	void orbit(int radius, float angle, glm::vec3 origin, glm::vec3 axis_of_orbit);
	void setTransformMatrix(glm::mat4& matrix, float angle, glm::vec3 vector_translate, glm::vec3 vector_rotate, glm::vec3 vector_scale);
	Shader shader;
	GLfloat *vertices;

	int arrayLength;
	int size_in_bits;

	unsigned int cube_VBO;
	unsigned int cube_VAO;
	
private:

};




#endif