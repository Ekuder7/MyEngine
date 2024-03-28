#include "Shader.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>

static GLuint load_shader(std::string vertexFile, std::string fragmentFile, std::ostringstream& ErrLog)
{
	// Reading Files
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);
	try
	{
		vShaderFile.open(vertexFile);
		fShaderFile.open(fragmentFile);
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		ErrLog << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		return 0;
	}
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
		ErrLog << "SHADER::VERTEX: compilation failed" << std::endl;
		ErrLog << infoLog << std::endl;
		return 0;
	}

	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
		ErrLog << "SHADER::FRAGMENT: compilation failed" << std::endl;
		ErrLog << infoLog << std::endl;
		return 0;
	}

	// Shader Program
	GLuint id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(id, 512, nullptr, infoLog);
		ErrLog << "SHADER::PROGRAM: linking failed" << std::endl;
		ErrLog << infoLog << std::endl;

		glDeleteShader(vertex);
		glDeleteShader(fragment);
		return 0;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return id;
}

Shader::Shader(std::string vertexFile, std::string fragmentFile)
{
	std::ostringstream ErrLog{};

	_ShaderProgramId = load_shader(vertexFile, fragmentFile, ErrLog);

	if (_ShaderProgramId == 0)
	{
		std::cerr << "Shader { " << vertexFile << ", " << fragmentFile << " } returned an error: " << std::endl << ErrLog.str() << std::endl;
	}
}

Shader::~Shader()
{
	glDeleteProgram(_ShaderProgramId);
}

void Shader::use()
{
	glUseProgram(_ShaderProgramId);
}

void Shader::uniformMatrix(std::string name, glm::mat4 matrix)
{
	GLuint transformLoc = glGetUniformLocation(_ShaderProgramId, name.c_str());
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));
}