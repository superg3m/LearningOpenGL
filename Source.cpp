#include <iostream>
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "./shaderClass.h"
#include "./stb_image.h"

const int WIDTH = 800;
const int HEIGHT = 800;

// Processing inputs with the Method processInput
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
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

	// Textures
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate the texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load("wall.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	


	GLfloat vertices[] =
	{
		//	  X		 Y		 Z
			// positions                colors         texture coords
			+0.0f,	+0.40f, +0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top
			-0.25f,	+0.0f,  +0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // middle left
			+0.25f,	+0.0f,  +0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // middle right

			-0.5f,	-0.4f,  +0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
			+0.0f,	-0.4f,  +0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // bottom middle

			+0.5f,	-0.4f,  +0.0f,  0.0f, 1.0f, 1.0f, 0.0f, 1.0f, // bottom right
	};


	// Indices
	GLuint indices[] =
	{
		0, 1, 2,	// first triangle
		1, 3, 4,	// second triangle
		2, 4, 5,	// third triangle
		1, 2, 4

	};

	GLfloat texCoords[] = {
		0.0f, 0.0f,  // lower-left corner  
		1.0f, 0.0f,  // lower-right corner
		0.5f, 1.0f   // top-center corner
	};

	GLfloat borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);


	Shader shaderProgram("./default.vert", "./default.frag");

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{

		// input
		processInput(window);

		// render background
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// render the triangle
		shaderProgram.use();
		glBindTexture(GL_TEXTURE, texture);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

		/* Update the uniform color
		float time = glfwGetTime();

		float changingColorValue = sin(time) / 2.0f + 0.5f;
		float changingColorValue2 = cos(time) / 2.0f + 0.5f;
		float changingColorValue3 = sin(time / 0.5f + 0.5f) / 2.0f + 0.5f;

		float changingAlphaValue = sin(time) / 2.0f + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram.ID, "vertexColor");
		glUseProgram(shaderProgram.ID);
		glUniform4f(vertexColorLocation, changingColorValue3, changingColorValue2, changingColorValue, 1.0f);
		*/

		// Events and updates
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}