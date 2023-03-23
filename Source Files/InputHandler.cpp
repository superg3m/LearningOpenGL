#include <Headers/input_handler.h>

bool InputHandler::Mouse_One_Pressed;
bool InputHandler::Mouse_Two_Pressed;

InputHandler::InputHandler()
{
	this->Space_Key_Pressed = false;
	this->Escape_Key_Pressed = false;
	this->Left_Shift_Key_Pressed = false;
	this->Left_Control_Key_Pressed = false;

	this->W_Key_Pressed = false;
	this->S_Key_Pressed = false;
	this->A_Key_Pressed = false;
	this->D_Key_Pressed = false;
	this->L_Key_Pressed = false;
	this->V_Key_Pressed = false;
	this->F_Key_Pressed = false;

	this->Up_Arrow_Key_Pressed = false;
	this->Down_Arrow_Key_Pressed = false;
	this->Left_Arrow_Key_Pressed = false;
	this->Right_Arrow_Key_Pressed = false;

	this->hover = false;

	InputHandler::Mouse_One_Pressed = false;
	InputHandler::Mouse_Two_Pressed = false;

	
}

void InputHandler::processInput(GLFWwindow *window, Camera& camera, float& deltaTime, Shader &shader, glm::vec3 &lightPos, CMRSpline &splineObject)
{		
	#pragma region Changing Booleans
	// Non-letter Keys
	Left_Shift_Key_Pressed			= glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
	Escape_Key_Pressed				= glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
	Space_Key_Pressed				= glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
	Left_Control_Key_Pressed		= glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;

	// Letter
	W_Key_Pressed					= glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
	S_Key_Pressed					= glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
	A_Key_Pressed					= glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
	D_Key_Pressed					= glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
	L_Key_Pressed					= glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS;
	V_Key_Pressed					= glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS;
	F_Key_Pressed					= glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS;

	// Arrows
	Up_Arrow_Key_Pressed			= glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS;
	Down_Arrow_Key_Pressed			= glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS;
	Left_Arrow_Key_Pressed			= glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS;
	Right_Arrow_Key_Pressed			= glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS;

	// Mouse
	InputHandler::Mouse_One_Pressed	= glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS;
	InputHandler::Mouse_Two_Pressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
	#pragma endregion


	#pragma region Implementation
	// Non-letter Keys
	if (Escape_Key_Pressed)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (Left_Shift_Key_Pressed)						
	{
		camera.ProcessKeyboard(SPEEDMODIFER, deltaTime);
	}
	if (Space_Key_Pressed)							
	{
		camera.ProcessKeyboard(UP, deltaTime);
	}
	if (Left_Control_Key_Pressed && !hover)
	{
		camera.ProcessKeyboard(DOWN, deltaTime);
	}

	// Letter
	if (W_Key_Pressed)
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (S_Key_Pressed)
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (A_Key_Pressed)
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (D_Key_Pressed)
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (F_Key_Pressed)								
	{
		shader.setVec3("spotLight.specular", glm::vec3(100.0f));
		shader.setFloat("spotLight.flashLightMode", 1);
	}
	else
	{
		shader.setVec3("spotLight.specular", glm::vec3(1.0f));
		shader.setFloat("spotLight.flashLightMode", 0);
	}
	if (L_Key_Pressed)								
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	// Arrows
	if (Left_Arrow_Key_Pressed)						
	{
		lightPos.x -= (10 * deltaTime);
	}
	if (Right_Arrow_Key_Pressed)					
	{
		lightPos.x += (10 * deltaTime);
	}
	if (Down_Arrow_Key_Pressed && !V_Key_Pressed)	
	{
		lightPos.y -= (10 * deltaTime);
	}
	if (Up_Arrow_Key_Pressed && !V_Key_Pressed)		
	{
		lightPos.y += (10 * deltaTime);
	}

	if (Up_Arrow_Key_Pressed && V_Key_Pressed)		
	{
		lightPos.z -= (10 * deltaTime);
	}
	if (Down_Arrow_Key_Pressed && V_Key_Pressed)
	{
		lightPos.z += (10 * deltaTime);
	}
	if (InputHandler::Mouse_One_Pressed)
	{
		splineObject.drawSpline();
	}
	#pragma endregion
}