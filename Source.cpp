#include <iostream>
#include <glad/glad.h>
#include <glfw/glfw3.h>

const int WIDTH = 800;
const int HEIGHT = 600;

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
"   FragColor = vec4(0.8f, 0.4f, 0.8f, 1.0f);\n"
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
	glViewport(0, 0, WIDTH, HEIGHT);

	// Create a vertex shader object
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Vertex shader sources
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Compile the object into machine code
	glCompileShader(vertexShader);

	// Create a Fragment shader object
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Fragment shader Soruce
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// Compile the fragment shader object to machine code
	glCompileShader(fragmentShader);

	// Create a shader program
	GLuint shaderProgram = glCreateProgram();
	// Basically subscribing something to a delegate myDelegate += myMethod()
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// Links all the shaders together
	glLinkProgram(shaderProgram);

	// After you link them into the shader program you can delete the shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Vertices coordinates
	GLfloat vertices[] =
	{
		-0.0f, -0.5f, 0.0f, // Lower left corner
		0.5f, -0.5f, 0.0f, // Lower right corner
		0.0f, 0.5f, 0.0f // Upper corner
	};

	GLuint vertexArrayObject, vertexBufferObject;

	// VertexArrayObject need to be generated before the VertextBufferObject
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);





	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);



	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "GLAD did not initialize correctly";
		glfwTerminate();
		return -1;
	}

	while (!glfwWindowShouldClose(window)) {
		// Inputs
		processInput(window);

		// Rendering
		glClearColor(0.1f, 0.3f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(vertexArrayObject);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Events and buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vertexArrayObject);
	glDeleteBuffers(1, &vertexBufferObject);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(window, true);
	}
}