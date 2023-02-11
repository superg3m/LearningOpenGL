// C++ includes
#include <iostream>
#include <map>

// OpenGL includes
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

// Our includes
#include <Headers/shaderClass.h>
#include <Headers/stb_image.h>
#include <Headers/PositionData.h>
#include <Headers/admin.h>


// FreeType includes
#include <ft2build.h>
#include FT_FREETYPE_H  

// Methods

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void bindBuffers(unsigned int &generic_VBO, unsigned int& generic_VAO);
std::pair<float, float> circle_points(float radius, float angle, glm::vec2 origin);
void configureBufferAttributes(const int position, const int color, const int texture, const int normal, int& number_of_elements_per_line);
void configureTextures(unsigned int& texture, Shader& shader);
glm::mat4 transformMatrix(glm::mat4& matrix, float angle, glm::vec3 vector_translate, glm::vec3 vector_rotate, glm::vec3 vector_scale);
void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color);

// Constants
const bool TRANSLATION_ROTATION = false;
const bool RAINBOW_COLORS = false;

const float PI = 3.1415926;
const float TAU = 6.2831853;
const float LIGHT_ROTATION_SPEED = 100.0f;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

