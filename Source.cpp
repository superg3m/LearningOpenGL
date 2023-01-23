#include <iostream>
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

const int WIDTH = 800;
const int HEIGHT = 800;

// Processing inputs with the Method processInput
void processInput(GLFWwindow* window);


int main() {
	// Initialize GLFW
	glfwInit();


	// GLFW hits the version to the compiler
	glfwWindowHint(GLFW_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_VERSION_MINOR, 3);

	// Make a Pointer to a GLFWwindow
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OPEN GL", NULL, NULL);

	// If Window is NULL then terminate the GLFW and return the program
	if (window == NULL) {
		std::cerr << "Window is NUL";
		glfwTerminate();
		return -1;
	}

	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL Needs to be after the context Current
	gladLoadGL();

	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 600
	// Area of the window we want OpenGL to render
	glViewport(0, 0, WIDTH, HEIGHT);

	


	/* 
		- Sending data from GPU to CPU is slow so we need to send them in big batches which are called buffers
		- Binding in OpenGL means that you make a certain object the current object
	*/

	// Vertices coordinates

	/*
			   /\
			  /  \
			 /    \
			/______\
	*/
	
	GLfloat vertices[] =
	{
	//	  X		 Y		 Z
		// First Triangle
		+0.0f,	+0.40f,	+0.0f,  // top
		-0.25f,	+0.0f,	+0.0f,  // left
		+0.25f,	+0.0f,	+0.0f,  // right

		// Second Triangle
		-0.5f,	-0.4f,	+0.0f,  // left
		+0.0f,	-0.4f,	+0.0f,  // right

		// Third Triangle
		+0.5f,	-0.4f,	+0.0f,  // right
	};	

	// Indices
	GLuint indices[] =
	{
		0, 1, 2,	// first triangle
		1, 3, 4,	// second triangle
		2, 4, 5,		// third triangle
		1, 2, 4

	};

	Shader shaderProgram("default.vert", "default.frag");

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	VAO1.LinkVBO(VBO1, 0);
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "GLAD did not initialize correctly";
		glfwTerminate();
		return -1;
	}

	while (!glfwWindowShouldClose(window)) {
		// Inputs
		processInput(window);

		// Rendering
		glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderProgram.Activate();
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// Events, buffers, and Resizing 
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// Deletes all the objects previously created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	// Destroys the Window
	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(window, true);
	}
}