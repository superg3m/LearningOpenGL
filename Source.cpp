#include <iostream>
#include <glad/glad.h>
#include <glfw/glfw3.h>

const int WIDTH = 800;
const int HEIGHT = 800;

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.0f, 0.8f, 0.4f, 1.0f);\n"
"}\n\0";

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

	// Create a vertex shader object
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Create a Fragment shader object
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Create a shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// Links all the shaders together
	glLinkProgram(shaderProgram);

	// After you link them into the shader program you can delete the shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

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
		2, 4, 5		// second triangle
	};
	

	GLuint VAO, VBO, EBO;

	// VertexArrayObject need to be generated before the VertextBufferObject
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// glVertexAttribPointer(position of the VAO, # of values per vertex, GL_FLOAT, GL_FALSE, Amount of Data between each vertex, Offset: Where the vertex begins in the array);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
	// glEnableVertexAttribArray(position of VAO);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


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
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// Events, buffers, and Resizing 
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

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