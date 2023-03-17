#include<Headers/main.h>
#include<Headers/camera.h>
#include <time.h>

enum Descriptive
{
	TexturePath,
	TextureID
};

unsigned int lightNodeIndex = 0; // Implement a tab change

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

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
	glfwSetWindowPos(window, SCREEN_WIDTH / 16, SCREEN_HEIGHT / 16);

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
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();

		return -1;
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_DEPTH_TEST);

	// *************** GUI ***************
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	stbi_set_flip_vertically_on_load(true);
	#pragma endregion

	// *************** Main Cube ********************
	#pragma region Main Cube
	Shader cubeShader("Shaders/Vertex/main_cube.vert", "Shaders/Fragment/main_cube.frag");

	Texture_Class cubeTexture;
	cubeTexture.addTexture("Textures/wall.jpg");
	cubeTexture.addTexture("Textures/container2_specular.png");
	cubeShader.use();
	cubeShader.setInt("material.texture_diffuse1", 0);
	cubeShader.setInt("material.texture_specular1", 1);

	unsigned int main_cube_VBO, main_cube_VAO;
	const int size_in_bits = sizeof(vertices_with_color);
	const int arrLength = size_in_bits / sizeof(GLfloat);
	int number_of_elements_per_line = arrLength / 36; // Finding the number of elements per Line

	bindBuffers(main_cube_VBO, main_cube_VAO);

	glBufferData(GL_ARRAY_BUFFER, size_in_bits, vertices_with_color, GL_STATIC_DRAW);
	configureBufferAttributes(3, 3, 2, 3, number_of_elements_per_line);
	#pragma endregion

	// *************** Light Cube *******************
	#pragma region Light Cube
	Shader lightCubeShader("Shaders/Vertex/light_cube.vert", "Shaders/Fragment/light_cube.frag");
	unsigned int light_cube_VBO, light_cube_VAO;
	int lightArrLength = sizeof(light_cube_vertices) / sizeof(GLfloat);
	number_of_elements_per_line = lightArrLength / 36; // Finding the number of elements per Line

	bindBuffers(light_cube_VBO, light_cube_VAO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(light_cube_vertices), light_cube_vertices, GL_STATIC_DRAW);

	configureBufferAttributes(3, NULL, NULL, NULL, number_of_elements_per_line);
	#pragma endregion

	

	// *************** FreeType ***************
	#pragma region FreeType
	Shader textShader("Shaders/Vertex/text.vert", "Shaders/Fragment/text.frag");
	Character freeTypeObject;

	textShader.use();
	glm::mat4 textProjection = glm::ortho(0.0f, static_cast<float>(SCREEN_WIDTH), 0.0f, static_cast<float>(SCREEN_HEIGHT));
	textShader.setMat4("projection", textProjection);

	freeTypeObject.init();

	freeTypeObject.deleteObjects();

	bindBuffers(freeTypeObject.text_VBO, freeTypeObject.text_VAO);
	freeTypeObject.bind();
	#pragma endregion

	// *************** Model ***************
	Model modelObject("../../3D/Neon.fbx");

	bool drawCubes = false;
	float cubeSize = 1.0f;

	std::vector<glm::vec3> points;
	points.push_back(glm::vec3(2, 0, 1));
	points.push_back(glm::vec3(3, 3, 1));
	points.push_back(glm::vec3(5, 1, 0));
	points.push_back(glm::vec3(6, 2, 1));
	points.push_back(glm::vec3(6, 2, 2));
	points.push_back(glm::vec3(7, 3, 1));

	std::vector<glm::vec3> splinePoints;

	bool pause = false;

	int spline_point_index = 0;
	std::vector<std::string> items;
	
	
	std::string current_item = "0";

	const unsigned int amount = 100;

	CMRSpline splineObject;

	// Clear the spline points after

	std::vector<glm::vec3> distanceVec;

	srand(time(NULL));

	// *************** Render Loop ***************
	#pragma region Render Loop
	while (!glfwWindowShouldClose(window))
	{
		#pragma region Initial Logic
		float currentTime = (float)glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		unsigned int FPS = 1 / deltaTime;
		std::string fpsText = std::to_string(FPS);

		cubeShader.use();

		
		if (!points.empty())
		{
			input.processInput(window, camera, deltaTime, cubeShader, points[spline_point_index]);
		}
		else
		{
			input.processInput(window, camera, deltaTime, cubeShader, lightNodePositions[0]);
		}
		
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		#pragma endregion

		// SPLINE CALCULATION
		items.clear();
		if (!pause)
		{
			splinePoints.clear();
			distanceVec.clear();
		}
		
		for (int i = 0; i < points.size(); i++)
		{
			items.push_back(std::to_string(i));
		}

		// Number of splines
		int number_of_splines = points.size() - 3;

		if (number_of_splines > 0 && !pause)
		{
			
			for (int i = 0; i < number_of_splines; i++)
			{
				for (int j = 0; j < amount; j++)
				{
					splinePoints.push_back(splineObject.CatmullRom(points[0], points[i + 1], points[i + 2], points[points.size() - 1], j / (float)amount));
				}
			}
			for (unsigned int i = 0; i < splinePoints.size() - 1; i++)
			{
				distanceVec.push_back(splinePoints[i + 1] - splinePoints[i]);
			}
			distanceVec.push_back(-distanceVec[distanceVec.size() - 1]);
			
		}

		// *************** Render the main cube ***************
		#pragma region Draw Main Cube
		// *************** Textures *********************
		cubeShader.setVec3("viewPos", camera.Position);
		cubeShader.setFloat("material.shininess", 64.0f);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		cubeShader.setMat4("projection", projection);
		cubeShader.setMat4("view", view);

		glm::mat4 model_main_cube = glm::mat4(1.0f);
		cubeShader.setMat4("model", model_main_cube);

		cubeShader.setVec3("dirLight.direction", 0, -1.0f, 0); // The power of the sun in the palm of my hands
		cubeShader.setVec3("dirLight.ambient", glm::vec3(1.05f));
		cubeShader.setVec3("dirLight.diffuse", glm::vec3(0.25f));
		cubeShader.setVec3("dirLight.specular", glm::vec3(0.25f));

		for (int i = 0; i < 1; i++)
		{
			std::string lightNode = "lightNode[" + std::to_string(i) + "]"; // Totally stole this one from kyle But I had the for loop prior
			cubeShader.setVec3(lightNode + ".position", lightNodePositions[i]);
			cubeShader.setVec3(lightNode + ".ambient", glm::vec3(2.0f));
			cubeShader.setVec3(lightNode + ".diffuse", glm::vec3(0.25f));
			cubeShader.setVec3(lightNode + ".specular", glm::vec3(3.0f));

			cubeShader.setFloat(lightNode + ".constant", 1.0f);
			cubeShader.setFloat(lightNode + ".linear", 0.09f);
			cubeShader.setFloat(lightNode + ".quadratic", 0.032f);
		}

		// spotLight
		cubeShader.setVec3("spotLight.position", camera.Position);
		cubeShader.setVec3("spotLight.direction", camera.Front);
		cubeShader.setVec3("spotLight.ambient", glm::vec3(1.5f));
		cubeShader.setVec3("spotLight.diffuse", glm::vec3(0.05f));

		cubeShader.setFloat("spotLight.constant", 1.0f);
		cubeShader.setFloat("spotLight.linear", 0.09f);
		cubeShader.setFloat("spotLight.quadratic", 0.032f);
		cubeShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		cubeShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));


		glm::vec3 lightColor;
		const float frequency = 0.25f;
		lightColor.x = 0.5 * (sin(TAU * (glfwGetTime() * frequency)) / 2.0) + 0.5;
		lightColor.y = 0.5 * (sin(TAU * (glfwGetTime() * frequency + 1.0 / 3.0)) / 2.0) + 0.5;
		lightColor.z = 0.5 * (sin(TAU * (glfwGetTime() * frequency + 2.0 / 3.0)) / 2.0) + 0.5;

		cubeShader.setVec3("lightColor", lightColor);
		cubeShader.setFloat("material.type", 0);
		
		for (unsigned int i = 0; i < 9; i++)
		{
			glm::mat4 model_main_cube = glm::mat4(1.0f);
			model_main_cube = glm::translate(model_main_cube, cubePositions[i]);
			model_main_cube = glm::scale(model_main_cube, glm::vec3(0.75f));
			if (TRANSLATION_ROTATION)
			{
				float functionTranslation = sin(PI * currentTime / 2.0f) / 2.0f;
				model_main_cube = glm::translate(model_main_cube, glm::vec3(0.0f, functionTranslation, 0.0f));
				model_main_cube = glm::rotate(model_main_cube, currentTime / 2.0f, glm::vec3(1, 0, 0));
			}
			// render the cube
			model_main_cube = glm::scale(model_main_cube, glm::vec3(cubeSize));
			cubeShader.setMat4("model", model_main_cube);
			
			if (cubeTexture.currently_loaded_textures.size() == 0)
			{
				for (size_t i = 0; i < 2; i++)
				{
					glActiveTexture(GL_TEXTURE0 + i);
					glBindTexture(GL_TEXTURE_2D, NULL);
				}
				
			}
			else 
			{
				for (size_t i = 0; i < cubeTexture.currently_loaded_textures.size(); i++)
				{
					glActiveTexture(GL_TEXTURE0 + i);
					glBindTexture(GL_TEXTURE_2D, std::get<TextureID>(cubeTexture.currently_loaded_textures[i]));
				}
			}
			
			glBindVertexArray(main_cube_VAO);
			if (drawCubes)
			{
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}
		for (int i = 0; i < splinePoints.size(); i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, splinePoints[i]);
			
			glm::quat quat = safeQuatLookAt(distanceVec[i], glm::vec3(0, 1, 0));
			glm::mat4 RotationMatrix = glm::toMat4(quat);
			model = model * RotationMatrix;
			model = glm::scale(model, glm::vec3(0.5f));
			cubeShader.setMat4("model", model);
			//cubeShader.setVec3("lightColor", glm::vec3(0.25f));
			//cubeShader.setFloat("material.type", 1);
			//cubeShader.setVec4("uniColor", glm::vec4(0.2, 1.0, 0.2f, 1.0));
			modelObject.Draw(cubeShader);
		}
		
		
		

		#pragma endregion

		// *************** Render the light cube ***************
		#pragma region Draw Light Cube
		lightCubeShader.use();
		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);
		lightCubeShader.setVec3("uniColor2", lightColor);

		glBindVertexArray(light_cube_VAO);
		for (unsigned int i = 0; i < points.size(); i++)
		{
			glm::mat4 model_light_cube = glm::mat4(1.0f);
			model_light_cube = glm::translate(model_light_cube, points[i]);
			model_light_cube = glm::scale(model_light_cube, glm::vec3(0.1f)); // Make it a smaller cube

			if (ORBIT)
			{
				std::pair<float, float> tmpPoint = circle_points(4.0f, glm::radians(LIGHT_ROTATION_SPEED * currentTime), glm::vec2(0.0f, 0.0f));
				lightNodePositions[i].x = tmpPoint.first;
				lightNodePositions[i].z = tmpPoint.second;
				model_light_cube = transformMatrix(model_light_cube,LIGHT_ROTATION_SPEED * currentTime, lightNodePositions[i], glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.1f));
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

		ImGui::Begin("Sprite GUI window");

		ImGui::Text("HELLO THERE!");
		ImGui::Checkbox("Draw Cubes", &drawCubes);

		if (ImGui::Button("Add Spline Points"))
		{
			int random_number_x = rand() % 5 + 1;
			int random_number_y = rand() % 5 + 1;
			int random_number_z = rand() % 5 + 1;
			if (!points.empty())
			{
				std::vector<glm::vec3>::iterator it = points.end() - 1;
				points.insert(it, glm::vec3(random_number_x, random_number_y, random_number_z));
			}
			else
			{
				points.push_back(glm::vec3(random_number_x, random_number_y, random_number_z));
			}
			pause = false;
		}
		if (ImGui::Button("Remove Spline Points"))
		{
			if (points.size() > 1)
			{
				std::vector<glm::vec3>::iterator it = points.end() - 2;
				points.erase(it);
			}
			else
			{
				points.clear();
			}
			pause = false;
		}
		if (ImGui::Button("Remove mesh"))
		{
			if (splinePoints.size() > 1)
			{
				std::vector<glm::vec3>::iterator it = splinePoints.end() - 1;
				splinePoints.erase(it);
			}
			else
			{
				splinePoints.clear();
			}
			pause = true;
		}
		if (ImGui::Button("Clear Spline Points"))
		{
			points.clear();
			pause = false;
		}

		if (!points.empty())
		{
			if (ImGui::BeginCombo("##combo", current_item.c_str())) // The second parameter is the label previewed before opening the combo.
			{
				for (int n = 0; n < items.size(); n++)
				{
					bool is_selected = (current_item == items[n]); // You can store your selection however you want, outside or inside your objects
					if (ImGui::Selectable(items[n].c_str(), is_selected))
					{
						current_item = items[n];
					}

					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
					}

				}
				ImGui::EndCombo();
			}
		}
		else
		{
			current_item = "0";
		}

		ImGui::SliderFloat("Cube Size", &cubeSize, 0.25f, 2.0f);
		std::string s = current_item;
		//std::cout << "String: " << s << std::endl;
		if (!points.empty())
		{
			spline_point_index = std::stoi(current_item);
			ImGui::SliderFloat("point-x-component", &points[spline_point_index].x, -10.0f, 10.0f);
			ImGui::SliderFloat("point-y-component", &points[spline_point_index].y, -10.0f, 10.0f);
			ImGui::SliderFloat("point-z-component", &points[spline_point_index].z, -10.0f, 10.0f);
		}
		

		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Events and updates
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	#pragma endregion

	#pragma region Delete Objects

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glDeleteVertexArrays(1, &main_cube_VAO);
	glDeleteVertexArrays(1, &light_cube_VAO);
	glDeleteBuffers(1, &main_cube_VBO);
	glDeleteBuffers(1, &light_cube_VBO);
	
#pragma endregion

	glfwTerminate();
	return 0;
}

#pragma region Methods
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	
	if (InputHandler::mousePressed)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		float xoffset = xposIn - SCREEN_WIDTH / 2;
		float yoffset = SCREEN_HEIGHT / 2 - yposIn; // reversed since y-coordinates go from bottom to to
		camera.ProcessMouseMovement(xoffset, yoffset);
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

std::pair<float, float> circle_points(const float &radius, const float &angle, const glm::vec2 &origin)
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

void configureBufferAttributes(const unsigned int &position, const unsigned  int &color, const unsigned int &texture, const unsigned int &normal, const unsigned int &number_of_elements_per_line)
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

glm::quat safeQuatLookAt(glm::vec3 &direction,glm::vec3 const& up)
{
	float directionLength = glm::length(direction);

	// Check if the direction is valid; Also deals with NaN
	if (!(directionLength > 0.0001))
		return glm::quat(1, 0, 0, 0); // Just return identity

	// Normalize direction
	direction = glm::normalize(direction);

	return glm::quatLookAt(direction, up);
}

#pragma endregion