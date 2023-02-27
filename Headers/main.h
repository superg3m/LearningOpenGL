// C++ includes
#include <iostream>
#include <map>
#include <tuple>

// OpenGL includes
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

// Our includes
#include <Headers/shader.h>
#include <Headers/stb_image.h>
#include <Headers/position_data.h>
#include <Headers/admin.h>
#include <Headers/character.h>
#include <Headers/model.h>

// Methods
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

void bindBuffers(unsigned int &generic_VBO, unsigned int& generic_VAO);
void configureBufferAttributes(const int position, const int color, const int texture, const int normal, int& number_of_elements_per_line);

std::pair<float, float> circle_points(float radius, float angle, glm::vec2 origin);
glm::mat4 transformMatrix(glm::mat4& matrix, float angle, glm::vec3 vector_translate, glm::vec3 vector_rotate, glm::vec3 vector_scale);

// Constants
const bool TRANSLATION_ROTATION = false;
const bool RAINBOW_COLORS = false;

const float PI = 3.1415926;
const float TAU = 6.2831853;
const float LIGHT_ROTATION_SPEED = 50.0f;

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;

