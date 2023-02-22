#ifndef CHARACTER_H
#define CHARACTER_H

#include <iostream>
#include <map>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H  

#include <Headers/shader.h>

class Character
{
public:
	unsigned int TextureID;  // ID handle of the glyph texture
	glm::ivec2   Size;       // Size of glyph
	glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
	unsigned int Advance;    // Offset to advance to next glyph

	FT_Library ft;
	FT_Face face;

	std::map<GLchar, Character> Characters;
	unsigned int text_VBO, text_VAO;

	void init();
	void bind();
	void deleteObjects();

	void renderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color);

private:


};
#endif