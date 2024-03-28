#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Mesh
{
public:
	Mesh(const GLfloat* buffer, size_t vertices, const int* attrs);
	~Mesh();

	void reload(const GLfloat* buffer, size_t vertices);
	void draw(GLenum drawMode);

private:
	GLuint VAO;
	GLuint VBO;

	size_t vertices;
	size_t vertex_size;
};