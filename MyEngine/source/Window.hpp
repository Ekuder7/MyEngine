#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace KEY
{
	inline bool* keys;
	inline uint32_t* frames;
	inline uint32_t current = 0;
	inline float deltaMouseX = 0.f;
	inline float deltaMouseY = 0.f;
	inline float MouseX = 0.f;
	inline float MouseY = 0.f;
	inline bool cursor_locked = false;
	inline bool cursor_started = false;
}

class Window
{
public:
	Window(int width, int height, const char* title);
	~Window();

	bool isShouldClose();
	void ForceCloseWindow();
	void swapBuffers();

	//Events

	void PollEvents();
	bool KeyHold(int keycode);
	bool KeyCliked(int keycode);
	bool MouseKeyHold(int button);
	bool MouseKeyClicked(int button);
	void ToggleMouseLocked();

private:
	GLFWwindow* _window;
};