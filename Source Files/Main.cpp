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
	Model modelObject("../3D/Neon.fbx");
	std::vector<Mesh> cachedMeshes = modelObject.meshes;

	float cubeSize = 1.0f;
	float splineOffset = 0.0f;
	float prevSplineOffset = 0.0f;

	glm::vec3 currentLinePoint = glm::vec3(0.0f);
	glm::vec3 PreviousLinePoint = glm::vec3(0.0f);
	

	bool pause = false;
	bool drawCubes = false;
	bool apply = true;
	
	int n = 0.0;


	
	int spline_point_index = 0;
	int lineIndex = 0;
	std::vector<std::string> items;
	

	std::vector<std::string> lineItems;
	std::string current_line_item = "0";

	std::vector<Vertex> lineSegmentPoints;

	calculateLineSegmentPoints(modelObject.meshes[0].vertices, lineSegmentPoints); // Calculate this outside of this constructor also only do this once and just pass in as an argument
	for (int i = 0; i < lineSegmentPoints.size(); i++)
	{
		lineItems.push_back(std::to_string(i));
	}
	
	
	std::string current_item = "0";

	const unsigned int amount = 25;

	CMRSpline splineObject;
	splineObject.addControlPoints(glm::vec3(2, 0, 1));
	splineObject.addControlPoints(glm::vec3(3, 3, 1));
	splineObject.addControlPoints(glm::vec3(5, 1, 0));
	splineObject.addControlPoints(glm::vec3(6, 2, 1));
	splineObject.addControlPoints(glm::vec3(6, 2, 2));
	splineObject.addControlPoints(glm::vec3(7, 3, 1));
	// Clear the spline points after


	
	


	srand(time(NULL));
	int splineIndex = 0;
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

		
		if (!splineObject.controlPoints.empty())
		{
			input.processInput(window, camera, deltaTime, cubeShader, lineSegmentPoints[lineIndex].Position, splineObject);
		}
		else
		{
			input.processInput(window, camera, deltaTime, cubeShader, lightNodePositions[0], splineObject);
		}
		
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		#pragma endregion

		// SPLINE CALCULATION
		items.clear();
		
		for (int i = 0; i < splineObject.controlPoints.size(); i++)
		{
			items.push_back(std::to_string(i));
		}
		
		splineObject.calculate_number_of_meshes_per_spline(modelObject);
		splineObject.calculateSplinePoints(pause, apply, splineObject.drawing);

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
		cubeShader.setVec3("dirLight.ambient", glm::vec3(0.05f));
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
				model_main_cube = glm::rotate(model_main_cube, currentTime / 2.0f, glm::vec3(1, 1, 1));
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
		splineIndex = 0;
		int sum = -1;
		if (!splineObject.meshAmount.empty())
		{
			sum = splineObject.meshAmount[splineIndex];
		}
		
		for (int i = 0; i < modelObject.meshes.size(); i++)
		{
			PreviousLinePoint = currentLinePoint;
			currentLinePoint = lineSegmentPoints[lineIndex].Position;
			glm::vec3 calcPoint = glm::vec3(currentLinePoint.x - PreviousLinePoint.x, currentLinePoint.y - PreviousLinePoint.y, currentLinePoint.z);
			if (PreviousLinePoint != currentLinePoint)
			{
				/*
				for (int i = 0; i < modelObject.meshes[0].vertices.size(); i++)
				{
					for (int j = 0; j < lineSegmentPoints.size(); j++)
					{
						if (modelObject.meshes[0].vertices[i].Position.z == lineSegmentPoints[j].Position.z)
						{
							float x = glm::distance(lineSegmentPoints[j].Position.x, modelObject.meshes[0].vertices[i].Position.x);
							float y = glm::distance(lineSegmentPoints[j].Position.y, modelObject.meshes[0].vertices[i].Position.y);
							modelObject.meshes[0].distanceToLine.push_back(glm::vec2(x, y));
						}
					}
				}
				*/
				modelObject.meshes[i] = Mesh(modelObject.meshes[i].vertices, modelObject.meshes[i].indices, modelObject.meshes[i].textures, calcPoint, true);
				prevSplineOffset = splineOffset;
			}
		}
		
		
		for (int i = 0; i < 1; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f));
			//glm::quat quat;
			if (apply)
			{
				//std::cout << "I: " << i << std::endl;
				if (i != 0)
				{
					//quat = safeQuatLookAt(splineObject.distanceVec[i - 1], glm::vec3(0, 1, 0));
				}
				else
				{
					//quat = safeQuatLookAt(splineObject.distanceVec[0], glm::vec3(0, 1, 0));
				}
				//splineIndex++;
				//sum += splineObject.meshAmount[splineIndex];
			}
			else
			{
				//quat = safeQuatLookAt(splineObject.distanceVec[i], glm::vec3(0, 1, 0));
			}
			
			//glm::mat4 RotationMatrix = glm::toMat4(quat);

			//model = model * RotationMatrix;
			model = glm::scale(model, glm::vec3(1.0f));
			cubeShader.setMat4("model", model);
			
			cubeShader.setVec3("lightColor", glm::vec3(0.25f));
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
		for (unsigned int i = 0; i < lineSegmentPoints.size(); i++)
		{
			glm::mat4 model_light_cube = glm::mat4(1.0f);
			//model_light_cube = glm::translate(model_light_cube, modelObject.meshes[0].vertices[vertexIndex].Position);
			model_light_cube = glm::translate(model_light_cube, lineSegmentPoints[i].Position);
			model_light_cube = glm::scale(model_light_cube, glm::vec3(0.001f)); // Make it a smaller cube

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
		ImGui::Checkbox("Apply", &apply);
		if (ImGui::Button("Reset"))
		{
			for (int i = 0; i < modelObject.meshes.size(); i++)
			{
				modelObject.meshes[i] = Mesh(cachedMeshes[i].vertices, cachedMeshes[i].indices, cachedMeshes[i].textures);
			}
			splineOffset = 0.0;
		}
		/*
		
		if (ImGui::Button("Add Spline Points"))
		{
			int random_number_x = rand() % 5 + 1;
			int random_number_y = rand() % 5 + 1;
			int random_number_z = rand() % 5 + 1;
			if (!splineObject.controlPoints.empty())
			{
				std::vector<glm::vec3>::iterator it = splineObject.controlPoints.end();
				splineObject.controlPoints.insert(it, glm::vec3(random_number_x, random_number_y, random_number_z));
			}
			else
			{
				splineObject.controlPoints.push_back(glm::vec3(random_number_x, random_number_y, random_number_z));
			}
			pause = false;
		}
		
		
		if (ImGui::Button("Remove control Points"))
		{
			if (splineObject.controlPoints.size() > 1)
			{
				std::vector<glm::vec3>::iterator it = splineObject.controlPoints.end() - 1;
				splineObject.controlPoints.erase(it);
			}
			else
			{
				splineObject.controlPoints.clear();
			}
			pause = false;
		}
		if (ImGui::Button("Remove mesh"))
		{
			if (splineObject.splinePoints.size() > 1)
			{
				std::vector<glm::vec3>::iterator it = splineObject.splinePoints.end() - 1;
				splineObject.splinePoints.erase(it);
			}
			else
			{
				splineObject.splinePoints.clear();
			}
			pause = true;
		}
		if (ImGui::Button("Clear control Points"))
		{
			splineObject.controlPoints.clear();
			pause = false;
		}
		if (ImGui::Button("Print mesh amount"))
		{
			for (auto n : splineObject.meshAmount)
			{
				std::cout << "AMOUNT of meshes PER SPLINE: " << n << std::endl;
			}
		}
		*/

		/*
		if (!splineObject.controlPoints.empty())
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
		*/
		if (ImGui::BeginCombo("##combo", current_line_item.c_str())) // The second parameter is the label previewed before opening the combo.
		{
			for (int n = 0; n < lineItems.size(); n++)
			{
				bool is_selected = (current_line_item == lineItems[n]); // You can store your selection however you want, outside or inside your objects
				if (ImGui::Selectable(lineItems[n].c_str(), is_selected))
				{
					current_line_item = lineItems[n];
				}

				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
				}

			}
			ImGui::EndCombo();
		}
		
		lineIndex = std::stoi(current_line_item);


		ImGui::SliderFloat("Cube Size", &cubeSize, 0.25f, 2.0f);

		ImGui::SliderFloat("SplinePoint-X", &splineOffset, -0.01f, 0.01f);

		ImGui::SliderInt("n", &n, 0, modelObject.meshes[0].vertices.size() / 4.0);


		std::string s = current_item;
		//std::cout << "String: " << s << std::endl;
		if (!splineObject.controlPoints.empty())
		{
			spline_point_index = std::stoi(current_item);
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
	if (InputHandler::Mouse_Two_Pressed)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(window, SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
		float xoffset = xposIn - SCREEN_WIDTH / 2.0f;
		float yoffset = (SCREEN_HEIGHT / 2.0f) - yposIn; // reversed since y-coordinates go from bottom to to
		//std::cout << "X :" << xoffset << " | Y: " << yoffset << std::endl;
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
#pragma endregion