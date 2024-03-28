#include <iostream>

#include "Window.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/VoxelRenderer.hpp"
#include "Graphics/LineRenderer.hpp"
#include "Voxels/Chunk.hpp"
#include "Voxels/Chunks.hpp"

#include "Crosshair.hpp"

#include "globals.hpp"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

using namespace glm;

int main()
{
	Window win(1920, 1080, "Engine");

	Crosshair crosshair;

	Shader* mainShader = new Shader("res\\main.glslv", "res\\main.glslf");
	Shader* lineShader = new Shader("res\\lines.glslv", "res\\lines.glslf");
	Texture* tex = new Texture("res\\block.png");
	Camera* camera = new Camera(vec3(200, 20, 200), radians(70.f));

	Chunks* chunks = new Chunks(16, 10, 16);
	Mesh** meshes = new Mesh*[chunks->volume];
	for (size_t i = 0; i < chunks->volume; ++i)
		meshes[i] = nullptr;

	VoxelRenderer renderer(1024 * 1024 * 8);
	LineRenderer* line_renderer = new LineRenderer(256);

	glClearColor(0.6f, 0.62f, 0.65f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float lastTime = glfwGetTime();
	float delta = 0.0f;

	float camX = 0.0f;
	float camY = 0.0f;

	float speed = 10;

	while (!win.isShouldClose())
	{
		float currentTime = glfwGetTime();
		delta = currentTime - lastTime;
		lastTime = currentTime;

		if (win.KeyCliked(GLFW_KEY_TAB))
		{
			win.ToggleMouseLocked();
		}

		if (win.KeyHold(GLFW_KEY_W))
		{
			camera->position += camera->front * delta * speed;

			/*
			float xDirection = 0;
			float zDirection = 0;

			if (camera->front.x > 0)
			{
				xDirection = 1;
			}
			else if (camera->front.x < 0)
			{
				xDirection = -1;
			}

			if (camera->front.z > 0)
			{
				zDirection = 1;
			}
			else if (camera->front.z < 0)
			{
				zDirection = -1;
			}

			camera->position += vec3(xDirection * delta * speed, 0, zDirection * delta * speed);
			*/
		}
		if (win.KeyHold(GLFW_KEY_S))
		{
			camera->position -= camera->front * delta * speed;
		}
		if (win.KeyHold(GLFW_KEY_A))
		{
			camera->position -= camera->right * delta * speed;
		}
		if (win.KeyHold(GLFW_KEY_D))
		{
			camera->position += camera->right * delta * speed;
		}

		if (win.KeyHold(GLFW_KEY_SPACE))
		{
			//camera->position += camera->up * delta * speed / 2.f;

			camera->position += glm::vec3(0, delta * speed, 0);
		}
		
		if (KEY::cursor_locked)
		{
			camY += -KEY::deltaMouseY / globals::winow_height * 3;
			camX += -KEY::deltaMouseX / globals::winow_width * 3;

			if (camY < -radians(90.0f))
			{
				camY = -radians(90.0f);
			}
			if (camY > radians(90.0f))
			{
				camY = radians(90.0f);
			}

			camera->rotation = mat4(1.0f);
			camera->rotate(camY, camX, 0);
		}
		
		{
			vec3 end;
			vec3 norm;
			vec3 iend;
			voxel* vox = chunks->rayCast(camera->position, camera->front, 10.0f, end, norm, iend);
			if (vox != nullptr)
			{
				line_renderer->box(iend.x + 0.5f, iend.y + 0.5f, iend.z + 0.5f, 1.005f, 1.005f, 1.005f, 0, 0, 0, 0.5f);

				if (win.MouseKeyClicked(GLFW_MOUSE_BUTTON_1))
				{
					chunks->set((int)iend.x, (int)iend.y, (int)iend.z, 0);
				}

				if (win.MouseKeyClicked(GLFW_MOUSE_BUTTON_2))
				{
					chunks->set((int)(iend.x) + (int)(norm.x), (int)(iend.y) + (int)(norm.y), (int)(iend.z) + (int)(norm.z), 2);
				}
			}
		}

		Chunk* closes[27];
		for (size_t i = 0; i < chunks->volume; i++)
		{
			Chunk* chunk = chunks->chunks[i];

			if (!chunk->modded) continue;

			chunk->modded = false;

			if (meshes[i] != nullptr)
				delete meshes[i];

			for (int i = 0; i < 27; i++)
				closes[i] = nullptr;
			for (size_t j = 0; j < chunks->volume; ++j)
			{
				Chunk* other = chunks->chunks[j];

				int ox = other->x - chunk->x;
				int oy = other->y - chunk->y;
				int oz = other->z - chunk->z;

				if (abs(ox) > 1 || abs(oy) > 1 || abs(oz) > 1)
					continue;

				ox += 1;
				oy += 1;
				oz += 1;
				closes[(oy * 3 + oz) * 3 + ox] = other;
			}
			Mesh* mesh = renderer.render(chunk, (const Chunk**)closes, true);
			meshes[i] = mesh;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mainShader->use();
		mainShader->uniformMatrix("projview", camera->getProjection() * camera->getView());
		tex->bind();

		mat4 model(1.0f);
		for (size_t i = 0; i < chunks->volume; ++i)
		{
			Chunk* chunk = chunks->chunks[i];
			Mesh* mesh = meshes[i];
			model = glm::translate(mat4(1.0f), vec3(chunk->x * CHUNK_W + 0.5f, chunk->y * CHUNK_H + 0.5f, chunk->z * CHUNK_D + 0.5f));
			mainShader->uniformMatrix("model", model);
			mesh->draw(GL_TRIANGLES);
			//mesh->draw(GL_LINES);
		}

		crosshair.draw();

		lineShader->use();
		lineShader->uniformMatrix("projview", camera->getProjection() * camera->getView());
		glLineWidth(5.0f);
		line_renderer->render();

		win.PollEvents();
		win.swapBuffers();
	}

	mainShader->~Shader();
	lineShader->~Shader();
	tex->~Texture();
	chunks->~Chunks();
	crosshair.~Crosshair();
	for (size_t i = 0; i < chunks->volume; ++i)
		meshes[i]->~Mesh();

	delete mainShader;
	delete lineShader;
	delete tex;
	delete camera;
	delete chunks;
	delete[] meshes;

	return 0;
}