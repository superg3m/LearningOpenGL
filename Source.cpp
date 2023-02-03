#include <iostream>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <admin.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shaderClass.h>
#include <stb_image.h>

const float PI = 3.1415926;
const float TAU = 6.2831853;

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

	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	// Make a Pointer to a GLFWwindow
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OPEN GL", NULL, NULL);

	// If Window is NULL then terminate the GLFW and return the program
	if (window == NULL) {
		std::cerr << "Window is NULL";
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

	#pragma region Position Data
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
	float offSet = 0.0f;
	float divisor = 0.2f;

	GLfloat vertices[] =
	{
		//	  X		 Y		 Z
			// positions                colors         texture coords
			+0.00f,	+0.4f,  +0.0f,  +1.0f, +0.0f, +0.0f, (+0.00f + offSet) / divisor, (+0.4f + offSet) / divisor, // top
			-0.25f,	+0.0f,  +0.0f,  +0.0f, +1.0f, +0.0f, (-0.25f + offSet) / divisor, (+0.0f + offSet) / divisor, // middle left
			+0.25f,	+0.0f,  +0.0f,  +0.0f, +0.0f, +1.0f, (+0.25f + offSet) / divisor, (+0.0f + offSet) / divisor, // middle right

			-0.50f,	-0.4f,  +0.0f,  +0.0f, +0.0f, +1.0f, (-0.50f + offSet) / divisor, (-0.4f + offSet) / divisor, // bottom left
			+0.00f,	-0.4f,  +0.0f,  +1.0f, +0.0f, +0.0f, (+0.00f + offSet) / divisor, (-0.4f + offSet) / divisor, // bottom middle

			+0.50f,	-0.4f,  +0.0f,  +0.0f, +1.0f, +1.0f, (+0.50f + offSet) / divisor, (-0.4f + offSet) / divisor, // bottom right
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
	#pragma endregion

	#pragma region Shaders
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
	#pragma endregion

	#pragma region Textures
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

		GLfloat borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	#pragma endregion

	#pragma region Render Loop
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{

		// input
		processInput(window);

		// render background
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// GLM
		// create transformations
		
		float speed = glfwGetTime();
		float functionTranslation_y = sin(PI * speed / 2.0f) / 2.0f;
		glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		transform = glm::translate(transform, glm::vec3(0.0f, 0.0f + functionTranslation_y, 0.0f));
		transform = glm::rotate(transform, speed, glm::vec3(0.25f, 0.25f, 0.25f));

		unsigned int transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		// render the triangle
		shaderProgram.use();

		// GLM
		unsigned int transformLocation = glGetUniformLocation(shaderProgram.ID, "transform");
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));

		glBindTexture(GL_TEXTURE, texture);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

		// Update the uniform color
		float time = glfwGetTime();
		float period = 2.0f; // Bigger period means lower speed
		float brightnessFactor = sin(TAU * time / period) / 2.0f;

		float fac1 = TAU * time;
		float changingColorValue = (sinf(fac1 / period) / 2.0f) + 0.5f;
		changingColorValue *= brightnessFactor;

		float fac2 = TAU * (time + (1.0f * period / 3.0f));
		float changingColorValue2 = (sinf(fac2 / period) / 2.0f) + 0.5f;
		changingColorValue2 *= brightnessFactor;

		float fac3 = TAU * (time + (2.0f * period / 3.0f));
		float changingColorValue3 = (sin(fac3 / period) / 2.0f) + 0.5f;
		changingColorValue3 *= brightnessFactor;

		//DEBUG_WRAP(std::cout << "1: " << changingColorValue << " | 2: " << changingColorValue2 << " | 3: " << changingColorValue3 << "\n";);

		int vertexColorLocation = glGetUniformLocation(shaderProgram.ID, "uniColor");
		glUseProgram(shaderProgram.ID);
		glUniform4f(vertexColorLocation, changingColorValue3, changingColorValue2, changingColorValue, 1.0f);

		// Events and updates
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	#pragma endregion

	#pragma region Delete Objects
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	#pragma endregion

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}