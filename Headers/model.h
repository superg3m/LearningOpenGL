#ifndef  MODEL_H
#define MODEL_H
#include <iostream>
#include <vector>
#include <tuple>

#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Model
{
public:
	std::vector<glm::vec3> vertcies;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texture_coordinates;
	std::vector<std::tuple<int, int, int>> faces; //Subtract by one on each index
	void loadModel(const std::string modelPath);

private:


};


#endif

