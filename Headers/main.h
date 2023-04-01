// C++ includes
#include <iostream>
#include <map>
#include <tuple>
#include <memory>


// OpenGL includes
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>


// Our includes
#include <Headers/shader.h>
#include <Headers/stb_image.h>
#include <Headers/position_data.h>
#include <Headers/admin.h>
#include <Headers/character.h>
#include <Headers/input_handler.h>
#include <Headers/model.h>
#include <Headers/texture_class.h>
#include <imGUI/imgui.h>
#include <imGUI/imgui_impl_opengl3.h>
#include <imGUI/imgui_impl_glfw.h>
#include <Headers/cat_mull_rom.h>


// Method prototypes
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Methods
std::pair<float, float> circle_points(const float& radius, const float& angle, const glm::vec2& origin)
{
	float xPos = radius * sin(angle);
	float yPos = radius * cos(angle);
	return std::make_pair(xPos, yPos);
}

void bindBuffers(unsigned int& generic_VBO, unsigned int& generic_VAO)
{
	glGenBuffers(1, &generic_VBO);
	glGenVertexArrays(1, &generic_VAO);
	glBindVertexArray(generic_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, generic_VBO);
}

void configureBufferAttributes(const unsigned int& position, const unsigned  int& color, const unsigned int& texture, const unsigned int& normal, const unsigned int& number_of_elements_per_line)
{
	// TODO: redo this method better
	int offset = 0;
	if (position != NULL)
	{
		// Position attribute
		glVertexAttribPointer(0, position, GL_FLOAT, GL_FALSE, number_of_elements_per_line * sizeof(float), (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(0);
		offset += position;
	}
	if (color != NULL)
	{
		// Color attributoffset
		glVertexAttribPointer(1, color, GL_FLOAT, GL_FALSE, number_of_elements_per_line * sizeof(float), (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(1);
		offset += color;
	}
	if (texture != NULL)
	{
		// Texture Attribute
		glVertexAttribPointer(2, texture, GL_FLOAT, GL_FALSE, number_of_elements_per_line * sizeof(float), (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(2);
		offset += texture;
	}
	if (normal != NULL)
	{
		// Normal Attribute
		glVertexAttribPointer(3, normal, GL_FLOAT, GL_FALSE, number_of_elements_per_line * sizeof(float), (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(3);
		offset += normal;
	}
}

glm::mat4 transformMatrix(glm::mat4& matrix, float angle, glm::vec3 vector_translate, glm::vec3 vector_rotate, glm::vec3 vector_scale)
{
	matrix = glm::mat4(1.0f); // Identity matrix is important
	matrix = glm::translate(matrix, vector_translate);
	matrix = glm::rotate(matrix, glm::radians(angle), vector_rotate);
	matrix = glm::scale(matrix, vector_scale);
	return matrix;
}

glm::quat safeQuatLookAt(glm::vec3& direction, glm::vec3 const& up)
{
	float directionLength = glm::length(direction);

	// Check if the direction is valid; Also deals with NaN
	if (!(directionLength > 0.0001))
	{
		return glm::quat(1, 0, 0, 0); // Just return identity
	}
	// Normalize direction
	glm::vec3 normDirection = glm::normalize(direction);
	return glm::quatLookAt(normDirection, up);
}

void calculateLineSegmentPoints(std::vector<Vertex> &verts, std::vector<Vertex> &lineSegmentPoints)
{
    std::vector<Vertex> cacheLower_X;
    std::vector<Vertex> cacheHigher_X;

    std::vector<Vertex> cacheLower_Y;
    std::vector<Vertex> cacheHigher_Y;

    std::vector<Vertex> cacheLower_Z;
    std::vector<Vertex> cacheHigher_Z;

    std::vector<float> positionsX;
    std::vector<float> positionsY;
    std::vector<float> positionsZ;
    for (int i = 0; i < verts.size(); i++)
    {
        positionsX.push_back(verts[i].Position.x);
        positionsY.push_back(verts[i].Position.y);
        positionsZ.push_back(verts[i].Position.z);
    }

    std::sort(positionsX.begin(), positionsX.end()); // Lowest to Largest
    std::sort(positionsY.begin(), positionsY.end());
    std::sort(positionsZ.begin(), positionsZ.end());
    for (int i = 0; i < verts.size(); i++)
    {
        if (verts[i].Position.y == positionsY.front())
        {
            cacheLower_Y.push_back(verts[i]);
        }
        if (verts[i].Position.y == positionsY.back())
        {
            cacheHigher_Y.push_back(verts[i]);
        }
    }

    for (int i = 0; i < cacheLower_Y.size(); i++)
    {
        for (int j = i + 1; j < cacheLower_Y.size(); j++)
        {
            if (cacheLower_Y[i].Position.z > cacheLower_Y[j].Position.z)
            {
                Vertex cache = cacheLower_Y[i];
                cacheLower_Y[i] = cacheLower_Y[j];
                cacheLower_Y[j] = cache;
            }
            if (cacheHigher_Y[i].Position.z > cacheHigher_Y[j].Position.z)
            {
                Vertex cache = cacheHigher_Y[i];
                cacheHigher_Y[i] = cacheHigher_Y[j];
                cacheHigher_Y[j] = cache;
            }
        }
    }
    for (int i = 0; i < cacheLower_Y.size(); i++)
    {
        float centerHeight = (cacheHigher_Y[i].Position.y + cacheLower_Y[i].Position.y) / 2.0;
        Vertex CalculatedVertex = cacheHigher_Y[i];
        CalculatedVertex.Position.y = centerHeight;
        lineSegmentPoints.push_back(CalculatedVertex);
    }
    for (int i = 0; i < lineSegmentPoints.size(); i++)
    {
        for (int j = i; j < lineSegmentPoints.size(); j++)
        {
            if (lineSegmentPoints[i].Position.x == lineSegmentPoints[j].Position.x && lineSegmentPoints[i].Position.y == lineSegmentPoints[j].Position.y && lineSegmentPoints[i].Position.z == lineSegmentPoints[j].Position.z)
            {
                auto it = lineSegmentPoints.begin() + j;
                lineSegmentPoints.erase(it);
            }
        }
    }
    auto it = lineSegmentPoints.end() - 1;
    lineSegmentPoints.erase(it);
}

// Constants
const bool TRANSLATION_ROTATION = true;
const bool ORBIT = false;

const float PI = 3.1415926;
const float TAU = 6.2831853;
const float LIGHT_ROTATION_SPEED = 50.0f;

const static int SCREEN_WIDTH = 1200;
const static int SCREEN_HEIGHT = 800;

