#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H    
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <Headers/camera.h>
#include <Headers/shader.h>
#include <Headers/cat_mull_rom.h>

class InputHandler
{
public:
	InputHandler();
	void processInput(GLFWwindow* window, Camera& camera, float& deltaTime, Shader& shader, glm::vec3& lightPos, CMRSpline &splineObejct);
	static bool Mouse_One_Pressed, Mouse_Two_Pressed;
private:
	// Non-Letter Keys
	bool Left_Shift_Key_Pressed, Escape_Key_Pressed, Space_Key_Pressed, Left_Control_Key_Pressed;

	// Letter Keys
	bool W_Key_Pressed, S_Key_Pressed, A_Key_Pressed, D_Key_Pressed, L_Key_Pressed, V_Key_Pressed, F_Key_Pressed;

	// Arrow Keys
	bool Up_Arrow_Key_Pressed, Down_Arrow_Key_Pressed, Left_Arrow_Key_Pressed, Right_Arrow_Key_Pressed;

	//Mouse Input
	
	
	bool hover;
};
#endif 