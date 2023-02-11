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

// FreeType
struct Character {
	unsigned int TextureID;  // ID handle of the glyph texture
	glm::ivec2   Size;       // Size of glyph
	glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
	unsigned int Advance;    // Offset to advance to next glyph
};
std::map<char, Character> Characters;
unsigned int text_VBO, text_VAO;

int main() {

	// *************** Initialization ***************
	#pragma region inits
	// Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	// Make a Pointer to a GLFWwindow
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OPEN GL", NULL, NULL);
	glfwSetWindowPos(window, SCREEN_WIDTH / 8 , SCREEN_HEIGHT / 8);

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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	#pragma endregion


	// *************** Shaders ***************
	#pragma region Shaders

	
	#pragma region Main Cube
	// first, configure the cube's VAO (and VBO)
	Shader lightingShader("Shaders/Vertex/light_color.vert", "Shaders/Fragment/light_color.frag");
	unsigned int main_cube_VBO, main_cube_VAO;

	int arrLength = sizeof(vertices_without_color) / sizeof(GLfloat);
	int number_of_elements_per_line = arrLength / 36; // Finding the number of elements per Line

	bindBuffers(main_cube_VBO, main_cube_VAO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_without_color), vertices_without_color, GL_STATIC_DRAW);
	configureBufferAttributes(3, 3, 2, 3, number_of_elements_per_line);
	#pragma endregion

	// *************** Light Cube ***************
	#pragma region Light Cube
	Shader lightCubeShader("Shaders/Vertex/light_cube.vert", "Shaders/Fragment/light_cube.frag");
	unsigned int light_cube_VBO, light_cube_VAO;

	arrLength = sizeof(light_cube_vertices) / sizeof(GLfloat);
	number_of_elements_per_line = arrLength / 36; // Finding the number of elements per Line

	bindBuffers(light_cube_VBO, light_cube_VAO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(light_cube_vertices), light_cube_vertices, GL_STATIC_DRAW);
	configureBufferAttributes(3, NULL, NULL, NULL, number_of_elements_per_line);
	#pragma endregion

	// *************** Textures ***************
	#pragma region Textures
	// Textures
	unsigned int texture;
	configureTextures(texture, lightingShader);
	#pragma endregion

	// *************** FreeType ***************
	#pragma region FreeType
	Shader textShader("Shaders/Vertex/text.vert", "Shaders/Fragment/text.frag");
	glm::mat4 textProjection = glm::ortho(0.0f, static_cast<float>(SCREEN_WIDTH), 0.0f, static_cast<float>(SCREEN_HEIGHT));

	textShader.use();
	glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(textProjection));
	// FreeType
	// --------
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return -1;
	}

	// load font as face
	FT_Face face;
	if (FT_New_Face(ft, "Fonts/arial.ttf", 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return -1;
	}
	else {
		// set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, 48);

		// disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// load first 128 characters of ASCII set
		for (unsigned char c = 0; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}
			// generate texture
			unsigned int textTexture;
			glGenTextures(1, &textTexture);
			glBindTexture(GL_TEXTURE_2D, textTexture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// now store character for later use
			Character character = {
				textTexture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)
			};
			Characters.insert(std::pair<char, Character>(c, character));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);


	// configure VAO/VBO for texture quads
	// -----------------------------------
	bindBuffers(text_VBO, text_VAO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	#pragma endregion

	#pragma endregion

	// *************** Render Loop ***************
	#pragma region Render Loop
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		float currentTime = (float)glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		int FPS = 1 / deltaTime;\
		std::string fpsText = std::to_string(FPS);

		// input
		processInput(window);

		// render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		// Text Rendering
		RenderText(textShader, "OpenGL Project - Jovanni Djonaj", 25.0f, 25.0f, 1.0f, glm::vec3(0.2, 0.2f, 0.2f));
		RenderText(textShader, "FPS " + fpsText, SCREEN_WIDTH - 200.0f, SCREEN_HEIGHT - 50.0f, 1.00f, glm::vec3(0.3, 0.7f, 0.9f));
		

		// *************** Render the main cube ***************
		#pragma region Draw Main Cube
		glm::mat4 model_main_cube = glm::mat4(1.0f);

		// be sure to activate shader when setting uniforms/drawing objects
		lightingShader.use();
		glBindTexture(GL_TEXTURE_2D, texture); // Must bind textures before drawing
		lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		//lightingShader.setVec3("objectColor", 1.0f, 1.0f, 1.0f);
		lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		lightingShader.setVec3("lightPos", lightPos);
		lightingShader.setVec3("viewPos", camera.Position);

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

		#pragma region Color
		if (RAINBOW_COLORS)
		{
			// Update the uniform color
			float time = glfwGetTime();
			float period = 2.0f; // Bigger period means lower speed
			float brightnessFactor = sin(TAU * time / period) / 2.0f;

			float fac1 = TAU * time;
			float changingColorValue = (sinf(fac1 / period) / 2.0f) + 0.5f;
			changingColorValue *= brightnessFactor / 4;

			float fac2 = TAU * (time + (1.0f * period / 3.0f));
			float changingColorValue2 = (sinf(fac2 / period) / 2.0f) + 0.5f;
			changingColorValue2 *= brightnessFactor / 4;

			float fac3 = TAU * (time + (2.0f * period / 3.0f));
			float changingColorValue3 = (sin(fac3 / period) / 2.0f) + 0.5f;
			changingColorValue3 *= brightnessFactor / 4;

			//DEBUG_WRAP(std::cout << "1: " << changingColorValue << " | 2: " << changingColorValue2 << " | 3: " << changingColorValue3 << "\n";);

			int vertexColorLocation = glGetUniformLocation(lightingShader.ID, "uniColor");
			glUseProgram(lightingShader.ID);
			glUniform4f(vertexColorLocation, changingColorValue3, changingColorValue2, changingColorValue, 1.0f);
		}
		#pragma endregion

		for (unsigned int i = 0; i < 1; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			// world transformation
			float angle = 20.0f * i;
			lightingShader.setMat4("model", model_main_cube);

			model_main_cube = glm::translate(model_main_cube, cubePositions[i]);
			model_main_cube = glm::rotate(model_main_cube, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			// render the cube
			glBindVertexArray(main_cube_VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		if (TRANSLATION_ROTATION)
		{
			float functionTranslation = sin(PI * currentTime / 2.0f) / 2.0f;
			glm::mat4 transform = transformMatrix(transform, currentTime, glm::vec3(0.0f, functionTranslation, 0.0f), glm::vec3(0.5f, 0.25f, 0.25f), glm::vec3(0.5f, 0.5f, 0.5f));
			unsigned int transformLocation = glGetUniformLocation(lightingShader.ID, "transform");
			glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));
		}
		#pragma endregion

		// *************** Render the light cube ***************
		#pragma region Draw Light Cube
		// also draw the lamp object
		glm::mat4 model_light_cube = glm::mat4(1.0f);

		lightCubeShader.use();
		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);
		lightCubeShader.setMat4("model", model_light_cube);

		std::pair<float, float> tmpPoint = circle_points(4.0f, glm::radians(LIGHT_ROTATION_SPEED * currentTime), glm::vec2(0.0f, 0.0f));
		lightPos.x = tmpPoint.first;
		lightPos.z = tmpPoint.second;

		glm::mat4 transformTwo = transformMatrix(transformTwo, currentTime * LIGHT_ROTATION_SPEED, lightPos, glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.25f));

		unsigned int transformLocationTwo = glGetUniformLocation(lightCubeShader.ID, "transformTwo");
		glUniformMatrix4fv(transformLocationTwo, 1, GL_FALSE, glm::value_ptr(transformTwo));

		glBindVertexArray(light_cube_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
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
	#pragma endregion

	glfwTerminate();
	return 0;
}

#pragma region Methods
void processInput(GLFWwindow* window)
{
	const float camera_y_velocity = 0.025f;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard(FORWARD, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(BACKWARD, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(LEFT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) camera.Position.y -= camera_y_velocity;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera.Position.y += camera_y_velocity;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(LEFT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(RIGHT, deltaTime);

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

void configureTextures(unsigned int& texture, Shader& shader)
{
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
	shader.use();
	shader.setInt("texture", 0);
}

glm::mat4 transformMatrix(glm::mat4& matrix, float angle, glm::vec3 vector_translate, glm::vec3 vector_rotate, glm::vec3 vector_scale)
{
	matrix = glm::mat4(1.0f); // Identity matrix is important
	matrix = glm::translate(matrix, vector_translate);
	matrix = glm::rotate(matrix, glm::radians(angle), vector_rotate);
	matrix = glm::scale(matrix, vector_scale);
	return matrix;
}

void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color)
{
	// activate corresponding render state	
	shader.use();
	glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(text_VAO);

	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// update VBO for each character
		float textVertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, text_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(textVertices), textVertices); // be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
#pragma endregion