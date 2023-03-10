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

void bindBuffers(unsigned int &generic_VBO, unsigned int& generic_VAO);
void configureBufferAttributes(const unsigned int &position, const unsigned  int& color, const unsigned int& texture, const unsigned int& normal, const unsigned int& number_of_elements_per_line);

std::pair<float, float> circle_points(const float &radius, const float& angle, const glm::vec2& origin);
glm::mat4 transformMatrix(glm::mat4& matrix, float angle, glm::vec3 vector_translate, glm::vec3 vector_rotate, glm::vec3 vector_scale);

glm::quat QuatLookAt(glm::vec3 direction, glm::vec3 desiredUp);
glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest);

// Constants
const bool TRANSLATION_ROTATION = true;
const bool ORBIT = false;

const float PI = 3.1415926;
const float TAU = 6.2831853;
const float LIGHT_ROTATION_SPEED = 50.0f;

const static int SCREEN_WIDTH = 1200;
const static int SCREEN_HEIGHT = 800;

