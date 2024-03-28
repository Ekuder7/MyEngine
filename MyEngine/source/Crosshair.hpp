#pragma once

#include "Graphics/Shader.hpp"
#include "Graphics/Mesh.hpp"

class Crosshair
{
public:
	Crosshair()
	{
		float CrosshairVertices[] =
		{
			// x    y
		   0.00f,-0.02f,
		   0.00f, 0.02f,

		   -0.01f, 0.00f,
		   0.01f, 0.00f
		};
		int CrosshairAttrs[] =
		{
			2, 0
		};

		shader = new Shader("res\\crosshair.glslv", "res\\crosshair.glslf");
		mesh = new Mesh(CrosshairVertices, sizeof(CrosshairVertices) / sizeof(float) / 2, CrosshairAttrs);
	}

	~Crosshair()
	{
		shader->~Shader();
		mesh->~Mesh();

		delete shader;
		delete mesh;
	}

	void draw()
	{
		shader->use();
		mesh->draw(GL_LINES);
	}

private:
	Shader* shader;
	Mesh* mesh;
};