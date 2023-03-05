#pragma once
#include <iostream>
#include <tuple>
#include <vector>
#include <string>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <Headers/stb_image.h>

class Texture_Class
{
public:
	std::vector<std::tuple<std::string, unsigned int>> currently_loaded_textures;
    void addTexture(const std::string texturePath);

private:
    unsigned int loadTexture(const std::string texturePath);
};