#pragma once

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

class Shader
{
public:
	Shader(std::string vertexFile, std::string fragmentFile);
	~Shader();

	void use();
	void uniformMatrix(std::string name, glm::mat4 matrix);
private:
	GLuint _ShaderProgramId;
};