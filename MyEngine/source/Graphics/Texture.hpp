#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

class Texture
{
public:
	int width;
	int height;

	Texture(std::string filename);
	~Texture();

	void bind();

private:
	GLuint _TextureId;
};