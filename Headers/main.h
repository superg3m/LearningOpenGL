// C++ includes
#include <iostream>


// OpenGL includes
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Our includes
#include <Headers/shaderClass.h>
#include <Headers/stb_image.h>
#include<Headers/PositionData.h>
#include <Headers/admin.h>

// Methods
std::pair<float, float> circle_points(float radius, float angle, glm::vec3 origin);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// Constants
const bool tranlsations_rotations = false;
const bool rainbowColors = false;

const float PI = 3.1415926;
const float TAU = 6.2831853;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;