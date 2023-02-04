// C++ includes
#include <iostream>


// OpenGL includes
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Our includes
#include <shaderClass.h>
#include <stb_image.h>
#include<PositionData.h>
#include <admin.h>

// Methods
void processInput(GLFWwindow* window);

// Constants
const bool coolShit = true;
const bool rainbowColors = true;

const float PI = 3.1415926;
const float TAU = 6.2831853;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;