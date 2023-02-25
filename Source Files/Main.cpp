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
glm::vec3 lightPos(1.0f, 0.5f, 2.0f);

void inits()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif
}


int main() {
	// *************** Initialization ***************
	#pragma region inits
	// Initialize GLFW
	inits();

	

	// Make a Pointer to a GLFWwindow
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OPEN GL", NULL, NULL);
	glfwSetWindowPos(window, SCREEN_WIDTH / 16 , SCREEN_HEIGHT / 16);

	// If Window is NULL then terminate the GLFW and return the program
	if (window == NULL) {
		std::cerr << "Window is NULL";
		glfwTerminate();
		return -1;
	}

	// Introduce the window into the current context
	glfwMakeContextCurrent(window);
	//glfwSwapInterval(0);
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

	// *************** Shaders ***************
	#pragma region Shaders

	// *************** Main Cube ***************
	#pragma region Main Cube
	// first, configure the cube's VAO (and VBO)
	Shader cubeShader("Shaders/Vertex/main_cube.vert", "Shaders/Fragment/main_cube.frag");
	unsigned int main_cube_VBO, main_cube_VAO;
	int size_in_bits = sizeof(vertices_with_color);
	int arrLength = size_in_bits / sizeof(GLfloat);
	int number_of_elements_per_line = arrLength / 36; // Finding the number of elements per Line

	bindBuffers(main_cube_VBO, main_cube_VAO);

	GLfloat* verts = new GLfloat[arrLength];
	for (int i = 0; i < arrLength; i++)
	{
		verts[i] = vertices_with_color[i];
		//std::cout << verts[i] << "\n";
	}

	glBufferData(GL_ARRAY_BUFFER, size_in_bits, verts, GL_STATIC_DRAW);
	configureBufferAttributes(3, 3, 2, 3, number_of_elements_per_line);
	#pragma endregion

	// *************** Light Cube ***************
	#pragma region Light Cube
	Shader lightCubeShader("Shaders/Vertex/light_cube.vert", "Shaders/Fragment/light_cube.frag");
	unsigned int light_cube_VBO, light_cube_VAO;

	int lightArrLength = sizeof(light_cube_vertices) / sizeof(GLfloat);
	number_of_elements_per_line = lightArrLength / 36; // Finding the number of elements per Line

	bindBuffers(light_cube_VBO, light_cube_VAO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(light_cube_vertices), light_cube_vertices, GL_STATIC_DRAW);
	configureBufferAttributes(3, NULL, NULL, NULL, number_of_elements_per_line);
	#pragma endregion

	// *************** Textures ***************
	#pragma region Textures
	// Textures
	unsigned int texture;
	std::vector<unsigned int> diffuseMap;
	diffuseMap.push_back(loadTexture("Textures/wall.jpg", diffuseMap.size()));
	diffuseMap.push_back(loadTexture("Textures/container2.png", diffuseMap.size()));

	cubeShader.use();
	for (int i = 0; i < diffuseMap.size(); i++)
	{
		cubeShader.setInt("material.diffuse", 0);
	}
	
	#pragma endregion

	// *************** Materials ***************
	#pragma region Material properties of the main cube
	cubeShader.setVec3("material.ambient", glm::vec3(0.5f));
	cubeShader.setVec3("material.diffuse", glm::vec3(0.1f));
	cubeShader.setVec3("material.specular", glm::vec3(2.5f));
	cubeShader.setFloat("material.shininess", 32.0f);
	DEBUG_WRAP(cubeShader.setFloat("material.shininess", 0.0f););
	#pragma endregion
	
	// *************** FreeType ***************
	#pragma region FreeType
	Shader textShader("Shaders/Vertex/text.vert", "Shaders/Fragment/text.frag");
	Character freeTypeObject;
	glm::mat4 textProjection = glm::ortho(0.0f, static_cast<float>(SCREEN_WIDTH), 0.0f, static_cast<float>(SCREEN_HEIGHT));

	textShader.use();
	glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(textProjection));

	freeTypeObject.init();

	// destroy FreeType once we're finished
	freeTypeObject.deleteObjects();

	// configure VAO/VBO for texture quads
	// -----------------------------------
	//bindBuffers(freeTypeObject.text_VBO, freeTypeObject.text_VAO);
	glGenVertexArrays(1, &freeTypeObject.text_VAO);
	glGenBuffers(1, &freeTypeObject.text_VBO);
	glBindVertexArray(freeTypeObject.text_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, freeTypeObject.text_VBO);
	freeTypeObject.bind();
	#pragma endregion

	#pragma endregion


	
	// *************** Render Loop ***************
	#pragma region Render Loop
	while (!glfwWindowShouldClose(window))
	{
		#pragma region Initial Logic
		// per-frame time logic
		float currentTime = (float)glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		int FPS = 1 / deltaTime;
		std::string fpsText = std::to_string(FPS);

		// input
		processInput(window);

		// background
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		#pragma endregion

		// *************** Render the main cube ***************
		#pragma region Draw Main Cube
		glm::mat4 model_main_cube = glm::mat4(1.0f);
		

		// TODO put this into the cube object
		// be sure to activate shader when setting uniforms/drawing objects
		cubeShader.use();
		cubeShader.setVec3("light.position", lightPos);
		cubeShader.setVec3("viewPos", camera.Position);

		// light properties
		#pragma region Material Properties
		glm::vec3 lightColor;
		const float frequency = 0.25f;

		float xValue = 0.5 * (sin(TAU * (glfwGetTime() * frequency)) / 2.0) + 0.5;
		float yValue = 0.5 * (sin(TAU * (glfwGetTime() * frequency + 1.0/3.0)) / 2.0) + 0.5;
		float zValue = 0.5 * (sin(TAU * (glfwGetTime() * frequency + 2.0/3.0)) / 2.0) + 0.5;
		lightColor.x = static_cast<float>(xValue);
		lightColor.y = static_cast<float>(yValue);
		lightColor.z = static_cast<float>(zValue);

		glm::vec3 diffuseColor = lightColor * glm::vec3(1.0f); // influence of the light color
		glm::vec3 ambientColor = glm::vec3(0.1f);
		DEBUG_WRAP(ambientColor = glm::vec3(1.0f););
		cubeShader.setVec3("light.ambient", ambientColor);
		cubeShader.setVec3("light.diffuse", diffuseColor);
		cubeShader.setVec3("light.specular", glm::vec3(1.0f));

		cubeShader.setVec3("lightColor", lightColor);

		
		for (int i = 0; i < diffuseMap.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, diffuseMap[0]); // Must bind textures before drawing
		}
		
		#pragma endregion
		
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		cubeShader.setMat4("projection", projection);
		cubeShader.setMat4("view", view);

		#pragma region Color
		if (RAINBOW_COLORS)
		{
			// Update the uniform color
			float time = glfwGetTime();
			float period = 2.0f; // Bigger period means lower speed
			float brightnessFactor = sin(TAU * time / period) / 8.0f;
			float brightnessDivisor = 2;

			float fac1 = TAU * time;
			float changingColorValue = (sinf(fac1 / period) / 2.0f) + 0.5f;
			changingColorValue *= brightnessFactor / brightnessDivisor;

			float fac2 = TAU * (time + (1.0f * period / 3.0f));
			float changingColorValue2 = (sinf(fac2 / period) / 2.0f) + 0.5f;
			changingColorValue2 *= brightnessFactor / brightnessDivisor;

			float fac3 = TAU * (time + (2.0f * period / 3.0f));
			float changingColorValue3 = (sin(fac3 / period) / 2.0f) + 0.5f;
			changingColorValue3 *= brightnessFactor / brightnessDivisor;

			//DEBUG_WRAP(std::cout << "1: " << changingColorValue << " | 2: " << changingColorValue2 << " | 3: " << changingColorValue3 << "\n";);

			int vertexColorLocation = glGetUniformLocation(cubeShader.ID, "uniColor");
			glUseProgram(cubeShader.ID);
			glUniform4f(vertexColorLocation, changingColorValue3, changingColorValue2, changingColorValue, 1.0f);
		}
		#pragma endregion

		for (unsigned int i = 0; i < 1; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			// world transformation
			float angle = 20.0f * i;
			

			model_main_cube = glm::translate(model_main_cube, cubePositions[i]);
			//model_main_cube = glm::rotate(model_main_cube, glm::radians(0.0f), glm::vec3(0.0f,0.0f, 0.0f));
			
			if (TRANSLATION_ROTATION)
			{
				float functionTranslation = sin(PI * currentTime / 2.0f) / 2.0f;
				glm::mat4 transform = glm::mat4(1.0f);
				transform = transformMatrix(transform, currentTime * 20, glm::vec3(0.0f, functionTranslation, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.5f));
				cubeShader.setMat4("transform", transform);
			}
			// render the cube
			cubeShader.setMat4("model", model_main_cube);
			glBindVertexArray(main_cube_VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		
		#pragma endregion

		// TODO put this into the cube object
		// *************** Render the light cube ***************
		#pragma region Draw Light Cube
		// also draw the lamp object



		glm::mat4 model_light_cube = glm::mat4(1.0f);

		lightCubeShader.use();
		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);
		lightCubeShader.setMat4("model", model_light_cube);
		lightCubeShader.setVec3("uniColor2", lightColor);

		std::pair<float, float> tmpPoint = circle_points(4.0f, glm::radians(LIGHT_ROTATION_SPEED * currentTime), glm::vec2(0.0f, 0.0f));
		lightPos.x = tmpPoint.first;
		lightPos.z = tmpPoint.second;

		glm::mat4 transformTwo = transformMatrix(transformTwo, currentTime * LIGHT_ROTATION_SPEED, lightPos, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.25f));

		unsigned int transformLocationTwo = glGetUniformLocation(lightCubeShader.ID, "transformTwo");
		glUniformMatrix4fv(transformLocationTwo, 1, GL_FALSE, glm::value_ptr(transformTwo));

		glBindVertexArray(light_cube_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		#pragma endregion

		// *************** Render Text ***************
		#pragma region Render Text
		glEnable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Text Rendering
		freeTypeObject.renderText(textShader, "OpenGL Project - Jovanni Djonaj", 25.0f, 25.0f, 1.0f, glm::vec3(0.4, 0.4f, 0.4f));
		freeTypeObject.renderText(textShader, "FPS " + fpsText, SCREEN_WIDTH - 200.0f, SCREEN_HEIGHT - 50.0f, 1.00f, glm::vec3(0.3, 0.7f, 0.9f));

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glDisable(GL_CULL_FACE);
		glDisable(GL_BLEND);
		#pragma endregion

		// Events and updates
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	#pragma endregion

	#pragma region Delete Objects
	glDeleteVertexArrays(1, &main_cube_VAO);
	glDeleteVertexArrays(1, &light_cube_VAO);
	glDeleteBuffers(1, &main_cube_VBO);
	glDeleteBuffers(1, &light_cube_VBO);
	delete[] (verts);
	#pragma endregion

	glfwTerminate();
	return 0;
}

#pragma region Methods
void processInput(GLFWwindow* window)
{
	const float camera_y_velocity = 3.0f;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard(FORWARD, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(BACKWARD, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(LEFT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) camera.Position.y -= camera_y_velocity * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera.Position.y += camera_y_velocity * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(LEFT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) mousePressed = true;
	else
	{
		mousePressed = false;
	}
}

// glfw: whenever the mouse moves, this callback is called
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
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

std::pair<float, float> circle_points(float radius, float angle, glm::vec2 origin)
{
	float xPos = radius * sin(angle);
	float yPos = radius * cos(angle);
	return std::make_pair(xPos, yPos);
}

void bindBuffers(unsigned int& generic_VBO, unsigned int& generic_VAO)
{
	glGenBuffers(1, &generic_VBO);
	glGenVertexArrays(1, &generic_VAO);
	glBindVertexArray(generic_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, generic_VBO);
}

void configureBufferAttributes(const int position, const int color, const int texture, const int normal, int& number_of_elements_per_line)
{
	int offset = 0;
	if (position != NULL)
	{
		// Position attribute
		glVertexAttribPointer(0, position, GL_FLOAT, GL_FALSE, number_of_elements_per_line * sizeof(float), (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(0);
		offset += position;
	}
	if (color != NULL)
	{
		// Color attributoffset
		glVertexAttribPointer(1, color, GL_FLOAT, GL_FALSE, number_of_elements_per_line * sizeof(float), (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(1);
		offset += color;
	}
	if (texture != NULL)
	{
		// Texture Attribute
		glVertexAttribPointer(2, texture, GL_FLOAT, GL_FALSE, number_of_elements_per_line * sizeof(float), (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(2);
		offset += texture;
	}
	if (normal != NULL)
	{
		// Normal Attribute
		glVertexAttribPointer(3, normal, GL_FLOAT, GL_FALSE, number_of_elements_per_line * sizeof(float), (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(3);
		offset += normal;
	}
}

unsigned int loadTexture(const char* texturePath, int number_of_textures)
{
	unsigned int textureID;
	glActiveTexture(GL_TEXTURE0 + number_of_textures);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate the texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

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
	return textureID;
}

glm::mat4 transformMatrix(glm::mat4& matrix, float angle, glm::vec3 vector_translate, glm::vec3 vector_rotate, glm::vec3 vector_scale)
{
	matrix = glm::mat4(1.0f); // Identity matrix is important
	matrix = glm::translate(matrix, vector_translate);
	matrix = glm::rotate(matrix, glm::radians(angle), vector_rotate);
	matrix = glm::scale(matrix, vector_scale);
	return matrix;
}
#pragma endregion
