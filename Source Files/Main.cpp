#include<Headers/main.h>
#include<Headers/camera.h>

bool mousePressed = false;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;

// Time Variables
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Light Cube
glm::vec3 lightPos(1.2f, 0.5f, 2.0f);

int main() {
	// Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	#pragma region inits
	// Make a Pointer to a GLFWwindow
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OPEN GL", NULL, NULL);

	// If Window is NULL then terminate the GLFW and return the program
	if (window == NULL) {
		std::cerr << "Window is NULL";
		glfwTerminate();
		return -1;
	}

	// Introduce the window into the current context
	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Lose the cursor mode
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();

		return -1;
	}

	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 600
	// Area of the window we want OpenGL to render
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glEnable(GL_DEPTH_TEST);
	#pragma endregion

	#pragma region Shaders

	// first, configure the cube's VAO (and VBO)
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_with_color), vertices_with_color, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Texture Attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Light Shaders
	Shader lightingShader("Shaders/Vertex/light_color.vert", "Shaders/Fragment/light_color.frag");
	Shader lightCubeShader("Shaders/Vertex/light_cube.vert", "Shaders/Fragment/light_cube.frag");

	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	//glBufferData(GL_ARRAY_BUFFER, sizeof(light_cube_vertices), light_cube_vertices, GL_STATIC_DRAW);

	// we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Texture Attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	#pragma endregion

	#pragma region Textures
	// Textures
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate the texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load("Textures/wall.jpg", &width, &height, &nrChannels, 0);

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
	lightingShader.use();
	lightingShader.setInt("texture", 0);
	#pragma endregion

	#pragma region Render Loop
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// be sure to activate shader when setting uniforms/drawing objects
		lightingShader.use();
		//lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		lightingShader.setVec3("objectColor", 0.0f, 0.0f, 0.0f);  // Cube Colors
		lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

		// world transformation
		glm::mat4 model_main_cube = glm::mat4(1.0f);

		glm::mat4 model_light_cube = glm::mat4(1.0f);

		// render the cube
		#pragma region Color
		if (rainbowColors)
		{
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

			int vertexColorLocation = glGetUniformLocation(lightingShader.ID, "uniColor");
			glUseProgram(lightingShader.ID);
			glUniform4f(vertexColorLocation, changingColorValue3, changingColorValue2, changingColorValue, 1.0f);
		}
		#pragma endregion

		#pragma region Draw Main Cube
		if (tranlsations_rotations)
		{
			float speed = glfwGetTime();
			float functionTranslation = sin(PI * speed / 2.0f) / 2.0f;
			glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			transform = glm::translate(transform, glm::vec3(0.0f, 0.0f + functionTranslation, 0.0f));
			transform = glm::rotate(transform, speed, glm::vec3(0.5f, 0.25f, 0.25f));
			transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.5f));

			unsigned int transformLocation = glGetUniformLocation(lightingShader.ID, "transform");
			glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));
		}
		for (unsigned int i = 0; i < 10; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			// world transformation
			float angle = 20.0f * i;
			lightingShader.setMat4("model", model_main_cube);

			model_main_cube = glm::translate(model_main_cube, cubePositions[i]);
			model_main_cube = glm::rotate(model_main_cube, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			// render the cube
			glBindVertexArray(cubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		#pragma endregion

		#pragma region Draw Light Cube
		// also draw the lamp object
		lightCubeShader.use();
		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);
		
		model_light_cube = glm::translate(model_light_cube, lightPos);
		model_light_cube = glm::scale(model_light_cube, glm::vec3(0.2f)); // a smaller cube
		lightCubeShader.setMat4("model", model_light_cube);

		glBindVertexArray(lightCubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		#pragma endregion

		// Events and updates
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	#pragma endregion

	#pragma region Delete Objects
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightCubeVAO);
	glDeleteBuffers(1, &VBO);
	#pragma endregion

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	const float camera_y_velocity = 0.05f;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard(FORWARD, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(BACKWARD, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(LEFT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) camera.Position.y -= camera_y_velocity;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera.Position.y += camera_y_velocity;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) mousePressed = true;
	else
	{
		mousePressed = false;
	}
	
		
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);
	if (mousePressed)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;

		camera.ProcessMouseMovement(xoffset, yoffset);
	}
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}