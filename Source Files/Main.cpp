#include<Headers/main.h>
#include<Headers/camera.h>

enum Descriptive
{
	TexturePath,
	TextureID
};

int lightNodeIndex = 0; // Implement a tab change

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;

// Time Variables
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Light Cube
glm::vec3 lightPos(1.0f, 0.0f, 5.0f);

InputHandler input;


int main() {
	// *************** Initialization ***************
	#pragma region inits
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OPEN GL", NULL, NULL);
	glfwSetWindowPos(window, SCREEN_WIDTH / 16 , SCREEN_HEIGHT / 16);

	if (window == NULL) {
		std::cerr << "Window is NULL";
		glfwTerminate();
		return -1;
	}

	//glfwSwapInterval(0);
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

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_DEPTH_TEST); 	
	#pragma endregion

	// *************** Main Cube ********************
	#pragma region Main Cube
	// Make this a buffer Object
	unsigned int main_cube_VBO, main_cube_VAO;
	int size_in_bits = sizeof(vertices_with_color);
	int arrLength = size_in_bits / sizeof(GLfloat);
	int number_of_elements_per_line = arrLength / 36; // Finding the number of elements per Line

	Shader cubeShader("Shaders/Vertex/main_cube.vert", "Shaders/Fragment/main_cube.frag");

	GLfloat* verts = new GLfloat[arrLength];
	for (int i = 0; i < arrLength; i++)
	{
		verts[i] = vertices_with_color[i];
		//std::cout << verts[i] << "\n";
	}

	bindBuffers(main_cube_VBO, main_cube_VAO);

	glBufferData(GL_ARRAY_BUFFER, size_in_bits, verts, GL_STATIC_DRAW);
	configureBufferAttributes(3, 3, 2, 3, number_of_elements_per_line);
	#pragma endregion

	// *************** Light Cube *******************
	#pragma region Light Cube
	// Make this a buffer Object
	unsigned int light_cube_VBO, light_cube_VAO;
	int lightArrLength = sizeof(light_cube_vertices) / sizeof(GLfloat);
	number_of_elements_per_line = lightArrLength / 36; // Finding the number of elements per Line

	Shader lightCubeShader("Shaders/Vertex/light_cube.vert", "Shaders/Fragment/light_cube.frag");

	bindBuffers(light_cube_VBO, light_cube_VAO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(light_cube_vertices), light_cube_vertices, GL_STATIC_DRAW);

	configureBufferAttributes(3, NULL, NULL, NULL, number_of_elements_per_line);
	#pragma endregion

	// *************** Textures *********************
	cubeShader.addTexture("Textures/wall.jpg");
	cubeShader.addTexture("Textures/container2_specular.png");

	cubeShader.use();
	cubeShader.setInt("material.diffuse", 0);
	cubeShader.setInt("material.specular", 1);
	
	// *************** FreeType ***************
	#pragma region FreeType
	Shader textShader("Shaders/Vertex/text.vert", "Shaders/Fragment/text.frag");
	Character freeTypeObject;
	glm::mat4 textProjection = glm::ortho(0.0f, static_cast<float>(SCREEN_WIDTH), 0.0f, static_cast<float>(SCREEN_HEIGHT));

	textShader.use();
	glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(textProjection));

	freeTypeObject.init();

	freeTypeObject.deleteObjects();

	bindBuffers(freeTypeObject.text_VBO, freeTypeObject.text_VAO);
	freeTypeObject.bind();
	#pragma endregion
	
	// *************** Render Loop ***************
	#pragma region Render Loop
	while (!glfwWindowShouldClose(window))
	{
		#pragma region Initial Logic
		float currentTime = (float)glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		int FPS = 1 / deltaTime;
		std::string fpsText = std::to_string(FPS);

		cubeShader.use();
		input.processInput(window, camera, deltaTime, cubeShader, lightNodePositions[0]);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		

		#pragma endregion

		// *************** Render the main cube ***************
		#pragma region Draw Main Cube
		
		cubeShader.setVec3("viewPos", camera.Position);
		cubeShader.setFloat("material.shininess", 64.0f);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		cubeShader.setMat4("projection", projection);
		cubeShader.setMat4("view", view);

		glm::mat4 model_main_cube = glm::mat4(1.0f);
		cubeShader.setMat4("model", model_main_cube);

		cubeShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		cubeShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		cubeShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		cubeShader.setVec3("dirLight.specular", glm::vec3(1.0f));

		for (int i = 0; i < 4; i++)
		{
			std::string lightNode = "lightNode[" + std::to_string(i) + "]"; // Totally stole this one from kyle But I had the for loop prior
			cubeShader.setVec3(lightNode + ".position", lightNodePositions[i]);
			cubeShader.setVec3(lightNode + ".ambient", glm::vec3(0.1f));
			cubeShader.setVec3(lightNode + ".diffuse", glm::vec3(0.8f));
			cubeShader.setVec3(lightNode + ".specular", glm::vec3(50.0f));

			cubeShader.setFloat(lightNode + ".constant", 1.0f);
			cubeShader.setFloat(lightNode + ".linear", 0.09f);
			cubeShader.setFloat(lightNode + ".quadratic", 0.032f);
		}
		
		// spotLight
		cubeShader.setVec3("spotLight.position", camera.Position);
		cubeShader.setVec3("spotLight.direction", camera.Front);
		cubeShader.setVec3("spotLight.ambient", glm::vec3(1.5f));
		cubeShader.setVec3("spotLight.diffuse", glm::vec3(0.05f));
		//cubeShader.setVec3("spotLight.specular", glm::vec3(1.0f));

		cubeShader.setFloat("spotLight.constant", 1.0f);
		cubeShader.setFloat("spotLight.linear", 0.09f);
		cubeShader.setFloat("spotLight.quadratic", 0.032f);
		cubeShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		cubeShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		
		glm::vec3 lightColor;
		const float frequency = 0.25f;
		lightColor.x = 0.5 * (sin(TAU * (glfwGetTime() * frequency)) / 2.0) + 0.5;
		lightColor.y = 0.5 * (sin(TAU * (glfwGetTime() * frequency + 1.0/3.0)) / 2.0) + 0.5;
		lightColor.z = 0.5 * (sin(TAU * (glfwGetTime() * frequency + 2.0/3.0)) / 2.0) + 0.5;

		cubeShader.setVec3("lightColor", lightColor);

		// bind textures on corresponding texture units
		for (size_t i = 0; i < cubeShader.textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, std::get<TextureID>(cubeShader.textures[i]));
		}

		for (unsigned int i = 0; i < 9; i++)
		{
			glm::mat4 model_main_cube = glm::mat4(1.0f);
			model_main_cube = glm::translate(model_main_cube, cubePositions[i]);
			model_main_cube = glm::scale(model_main_cube, glm::vec3(0.75f));
			if (TRANSLATION_ROTATION)
			{
				float functionTranslation = sin(PI * currentTime / 2.0f) / 2.0f;
				model_main_cube = glm::translate(model_main_cube, glm::vec3(0.0f, functionTranslation, 0.0f));
				model_main_cube = glm::rotate(model_main_cube, currentTime / 2.0f, glm::vec3(1.0f, 1.0f, 0.0f));
			}
			// render the cube
			cubeShader.setMat4("model", model_main_cube);
			
			glBindVertexArray(main_cube_VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		#pragma endregion

		// *************** Render the light cube ***************
		#pragma region Draw Light Cube
		lightCubeShader.use();
		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);

		glm::mat4 model_light_cube = glm::mat4(1.0f);
		lightCubeShader.setMat4("model", model_light_cube);
		lightCubeShader.setVec3("uniColor2", lightColor);

		

		

		glBindVertexArray(light_cube_VAO); 
		for (unsigned int i = 0; i < 4; i++)
		{
			glm::mat4 model_light_cube = glm::mat4(1.0f);
			model_light_cube = glm::translate(model_light_cube, lightNodePositions[i]);
			model_light_cube = glm::scale(model_light_cube, glm::vec3(0.5f)); // Make it a smaller cube
			
			if (ORBIT)
			{
				std::pair<float, float> tmpPoint = circle_points(4.0f, glm::radians(LIGHT_ROTATION_SPEED * currentTime), glm::vec2(0.0f, 0.0f));
				lightPos.x = tmpPoint.first;
				lightPos.z = tmpPoint.second;
				glm::mat4 transformTwo = transformMatrix(transformTwo, 0, glm::vec3(lightPos), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f));
				lightCubeShader.setMat4("transformTwo", transformTwo);
			}
			lightCubeShader.setMat4("model", model_light_cube);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
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
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	glfwSetCursorPos(window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	if (InputHandler::mousePressed)
	{
		float xoffset = xposIn - SCREEN_WIDTH / 2;
		float yoffset = SCREEN_HEIGHT / 2 - yposIn; // reversed since y-coordinates go from bottom to to
		camera.ProcessMouseMovement(xoffset, yoffset);
	}
}

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

void configureBufferAttributes(const int position, const int color, const int texture, const int normal, int number_of_elements_per_line)
{
	// TODO: redo this method better
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

glm::mat4 transformMatrix(glm::mat4& matrix, float angle, glm::vec3 vector_translate, glm::vec3 vector_rotate, glm::vec3 vector_scale)
{
	matrix = glm::mat4(1.0f); // Identity matrix is important
	matrix = glm::translate(matrix, vector_translate);
	matrix = glm::rotate(matrix, glm::radians(angle), vector_rotate);
	matrix = glm::scale(matrix, vector_scale);
	return matrix;
}
#pragma endregion
