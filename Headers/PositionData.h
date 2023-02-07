#ifndef POSITION_DATA_H
#define POSITION_DATA_H

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float offSet = 0.0f;
float divisor = 0.2f;

GLfloat vertices_with_color[] =
{
	// X	 Y	    Z
	// positions                 colors         texture coords
	-0.5f, -0.5f, -0.5f,  +1.0f, +0.0f, +0.0f, 0.0f / divisor, 0.0f / divisor,
	+0.5f, -0.5f, -0.5f,  +0.0f, +1.0f, +0.0f, 1.0f / divisor, 0.0f / divisor,
	+0.5f, +0.5f, -0.5f,  +0.0f, +0.0f, +1.0f, 1.0f / divisor, 1.0f / divisor,

	+0.5f, +0.5f, -0.5f,  +0.0f, +0.0f, +1.0f, 1.0f / divisor, 1.0f / divisor,
	-0.5f, +0.5f, -0.5f,  +1.0f, +0.0f, +0.0f, 0.0f / divisor, 1.0f / divisor,
	-0.5f, -0.5f, -0.5f,  +0.0f, +1.0f, +1.0f, 0.0f / divisor, 0.0f / divisor,

	-0.5f, -0.5f, +0.5f,  +1.0f, +0.0f, +0.0f, 0.0f / divisor, 0.0f / divisor,
	+0.5f, -0.5f, +0.5f,  +0.0f, +1.0f, +0.0f, 1.0f / divisor, 0.0f / divisor,
	+0.5f, +0.5f, +0.5f,  +0.0f, +0.0f, +1.0f, 1.0f / divisor, 1.0f / divisor,

	+0.5f, +0.5f, +0.5f,  +0.0f, +0.0f, +1.0f, 1.0f / divisor, 1.0f / divisor,
	-0.5f,  0.5f, +0.5f,  +1.0f, +0.0f, +0.0f, 0.0f / divisor, 1.0f / divisor,
	-0.5f, -0.5f, +0.5f,  +0.0f, +1.0f, +1.0f, 0.0f / divisor, 0.0f / divisor,

	-0.5f, +0.5f, +0.5f,  +1.0f, +0.0f, +0.0f, 1.0f / divisor, 0.0f / divisor,
	-0.5f, +0.5f, -0.5f,  +0.0f, +1.0f, +0.0f, 1.0f / divisor, 1.0f / divisor,
	-0.5f, -0.5f, -0.5f,  +0.0f, +0.0f, +1.0f, 0.0f / divisor, 1.0f / divisor,

	-0.5f, -0.5f, -0.5f,  +0.0f, +0.0f, +1.0f, 0.0f / divisor, 1.0f / divisor,
	-0.5f, -0.5f, +0.5f,  +1.0f, +0.0f, +0.0f, 0.0f / divisor, 0.0f / divisor,
	-0.5f, +0.5f, +0.5f,  +0.0f, +1.0f, +1.0f, 1.0f / divisor, 0.0f / divisor,

	+0.5f, +0.5f, +0.5f,  +1.0f, +0.0f, +0.0f, 1.0f / divisor, 0.0f / divisor,
	+0.5f, +0.5f, -0.5f,  +0.0f, +1.0f, +0.0f, 1.0f / divisor, 1.0f / divisor,
	+0.5f, -0.5f, -0.5f,  +0.0f, +0.0f, +1.0f, 0.0f / divisor, 1.0f / divisor,

	+0.5f, -0.5f, -0.5f,  +0.0f, +0.0f, +1.0f, 0.0f / divisor, 1.0f / divisor,
	+0.5f, -0.5f, +0.5f,  +1.0f, +0.0f, +0.0f, 0.0f / divisor, 0.0f / divisor,
	+0.5f, +0.5f, +0.5f,  +0.0f, +1.0f, +1.0f, 1.0f / divisor, 0.0f / divisor,

	-0.5f, -0.5f, -0.5f,  +1.0f, +0.0f, +0.0f, 0.0f / divisor, 1.0f / divisor,
	+0.5f, -0.5f, -0.5f,  +0.0f, +1.0f, +0.0f, 1.0f / divisor, 1.0f / divisor,
	+0.5f, -0.5f, +0.5f,  +0.0f, +0.0f, +1.0f, 1.0f / divisor, 0.0f / divisor,

	+0.5f, -0.5f, +0.5f,  +0.0f, +0.0f, +1.0f, 1.0f / divisor, 0.0f / divisor,
	-0.5f, -0.5f, +0.5f,  +1.0f, +0.0f, +0.0f, 0.0f / divisor, 0.0f / divisor,
	-0.5f, -0.5f, -0.5f,  +0.0f, +1.0f, +1.0f, 0.0f / divisor, 1.0f / divisor,

	-0.5f, +0.5f, -0.5f,  +1.0f, +0.0f, +0.0f, 0.0f / divisor, 1.0f / divisor,
	+0.5f, +0.5f, -0.5f,  +0.0f, +1.0f, +0.0f, 1.0f / divisor, 1.0f / divisor,
	+0.5f, +0.5f, +0.5f,  +0.0f, +0.0f, +1.0f, 1.0f / divisor, 0.0f / divisor,

	+0.5f, +0.5f, +0.5f,  +0.0f, +0.0f, +1.0f, 1.0f / divisor, 0.0f / divisor,
	-0.5f, +0.5f, -0.5f,  +1.0f, +0.0f, +0.0f, 0.0f / divisor, 1.0f / divisor,
	-0.5f, +0.5f, +0.5f,  +0.0f, +1.0f, +1.0f, 1.0f / divisor, 1.0f / divisor,
};

GLfloat vertices_without_color[] =
{
	// X	 Y	    Z
	// positions                 colors         texture coords
	-0.5f, -0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 0.0f / divisor,
	+0.5f, -0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 0.0f / divisor,
	+0.5f, +0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 1.0f / divisor,

	+0.5f, +0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 1.0f / divisor,
	-0.5f, +0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 1.0f / divisor,
	-0.5f, -0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 0.0f / divisor,

	-0.5f, -0.5f, +0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 0.0f / divisor,
	+0.5f, -0.5f, +0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 0.0f / divisor,
	+0.5f, +0.5f, +0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 1.0f / divisor,

	+0.5f, +0.5f, +0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 1.0f / divisor,
	-0.5f, +0.5f, +0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 1.0f / divisor,
	-0.5f, -0.5f, +0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 0.0f / divisor,

	-0.5f, +0.5f, +0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 0.0f / divisor,
	-0.5f, +0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 1.0f / divisor,
	-0.5f, -0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 1.0f / divisor,

	-0.5f, -0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 1.0f / divisor,
	-0.5f, -0.5f, +0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 0.0f / divisor,
	-0.5f, +0.5f, +0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 0.0f / divisor,

	+0.5f, +0.5f, +0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 0.0f / divisor,
	+0.5f, +0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 1.0f / divisor,
	+0.5f, -0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 1.0f / divisor,

	+0.5f, -0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 1.0f / divisor,
	+0.5f, -0.5f, +0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 0.0f / divisor,
	+0.5f, +0.5f, +0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 0.0f / divisor,

	-0.5f, -0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 1.0f / divisor,
	+0.5f, -0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 1.0f / divisor,
	+0.5f, -0.5f, +0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 0.0f / divisor,

	+0.5f, -0.5f, +0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 0.0f / divisor,
	-0.5f, -0.5f, +0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 0.0f / divisor,
	-0.5f, -0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 1.0f / divisor,

	-0.5f, +0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 1.0f / divisor,
	+0.5f, +0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 1.0f / divisor,
	+0.5f, +0.5f, +0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 0.0f / divisor,

	+0.5f, +0.5f, +0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 0.0f / divisor,
	-0.5f, +0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 1.0f / divisor,
	-0.5f, +0.5f, +0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 1.0f / divisor,
};

float light_cube_vertices[] = {
	 0.5f, -0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 0.0f / divisor,
	 0.5f,  0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 0.0f / divisor,
	 0.5f,  0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 1.0f / divisor,

	-0.5f, -0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 1.0f / divisor,
	-0.5f,  0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 1.0f / divisor,
	-0.5f, -0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 0.0f / divisor,

	-0.5f, -0.5f,  0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 0.0f / divisor,
	 0.5f, -0.5f,  0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 0.0f / divisor,
	 0.5f,  0.5f,  0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 1.0f / divisor,

	 0.5f,  0.5f,  0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 1.0f / divisor,
	-0.5f,  0.5f,  0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 1.0f / divisor,
	-0.5f, -0.5f,  0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 0.0f / divisor,

	-0.5f,  0.5f,  0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 0.0f / divisor,
	-0.5f,  0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 1.0f / divisor,
	-0.5f, -0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 1.0f / divisor,

	-0.5f, -0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 1.0f / divisor,
	-0.5f, -0.5f,  0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 0.0f / divisor,
	-0.5f,  0.5f,  0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 0.0f / divisor,

	 0.5f,  0.5f,  0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 0.0f / divisor,
	 0.5f,  0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 1.0f / divisor,
	 0.5f, -0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 1.0f / divisor,

	 0.5f, -0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 1.0f / divisor,
	 0.5f, -0.5f,  0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 0.0f / divisor,
	 0.5f,  0.5f,  0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 0.0f / divisor,

	-0.5f, -0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 1.0f / divisor,
	 0.5f, -0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 1.0f / divisor,
	 0.5f, -0.5f,  0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 0.0f / divisor,

	 0.5f, -0.5f,  0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 0.0f / divisor,
	-0.5f, -0.5f,  0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 0.0f / divisor,
	-0.5f, -0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 1.0f / divisor,

	-0.5f,  0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 1.0f / divisor,
	 0.5f,  0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 1.0f / divisor,
	 0.5f,  0.5f,  0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 0.0f / divisor,

	 0.5f,  0.5f,  0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 0.0f / divisor,
	-0.5f,  0.5f,  0.5f,  +0.1f, +0.1f, +0.1f, 0.0f / divisor, 1.0f / divisor,
	-0.5f,  0.5f, -0.5f,  +0.1f, +0.1f, +0.1f, 1.0f / divisor, 1.0f / divisor,
};

// world space positions of our cubes
glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -6.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -8.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

#endif