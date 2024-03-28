#include <iostream>
#include "Window.hpp"

#include "globals.hpp"

#define _MOUSE_BUTTONS 1024

// callbacks
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (KEY::cursor_started)
	{
		KEY::deltaMouseX += xpos - KEY::MouseX;
		KEY::deltaMouseY += ypos - KEY::MouseY;
	}
	else
	{
		KEY::cursor_started = true;
	}
	KEY::MouseX = xpos;
	KEY::MouseY = ypos;
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mode)
{
	if (action == GLFW_PRESS)
	{
		KEY::keys[_MOUSE_BUTTONS + button] = true;
		KEY::frames[_MOUSE_BUTTONS + button] = KEY::current;
	}
	else if (action == GLFW_RELEASE)
	{
		KEY::keys[_MOUSE_BUTTONS + button] = false;
		KEY::frames[_MOUSE_BUTTONS + button] = KEY::current;
	}
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
	{
		KEY::keys[key] = true;
		KEY::frames[key] = KEY::current;
	}
	else if (action == GLFW_RELEASE)
	{
		KEY::keys[key] = false;
		KEY::frames[key] = KEY::current;
	}
}
void window_resize_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

	globals::winow_width = width;
	globals::winow_height = height;
}

Window::Window(int width, int height, const char* title)
{
	globals::winow_width = width;
	globals::winow_height = height;

	if (glfwInit() == GLFW_FALSE)
	{
		std::cerr << "GLFW: [glfwInit()] returned an error" << std::endl;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (_window == nullptr)
	{
		std::cerr << "GLFW: [glfwCreateWindow()] returned an error" << std::endl;

		glfwTerminate();
	}

	glfwMakeContextCurrent(_window);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "GLEW: [glewInit()] returned an error" << std::endl;

		glfwTerminate();
	}

	//glfwSwapInterval(1);
	glViewport(0, 0, width, height);

	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	std::cout << "GL Vendor: " << (char*)vendor << std::endl;
	std::cout << "GL Renderer: " << (char*)renderer << std::endl;
	std::cout << "GLFW: " << glfwGetVersionString() << std::endl;

	// Events initialization
	KEY::keys = new bool[1032];
	KEY::frames = new uint32_t[1032];

	memset(KEY::keys, false, 1032 * sizeof(bool));
	memset(KEY::frames, 0, 1032 * sizeof(uint32_t));

	glfwSetKeyCallback(_window, key_callback);
	glfwSetMouseButtonCallback(_window, mouse_button_callback);
	glfwSetCursorPosCallback(_window, cursor_position_callback);
	glfwSetWindowSizeCallback(_window, window_resize_callback);
}

Window::~Window()
{
	glfwDestroyWindow(_window);
	glfwTerminate();

	delete[] KEY::keys;
	delete[] KEY::frames;
}

bool Window::isShouldClose()
{
	return glfwWindowShouldClose(_window);
}

void Window::ForceCloseWindow()
{
	glfwSetWindowShouldClose(_window, true);
}

void Window::swapBuffers()
{
	glfwSwapBuffers(_window);
}

//Events

void Window::PollEvents()
{
	KEY::current++;
	KEY::deltaMouseX = 0.0f;
	KEY::deltaMouseY = 0.0f;
	glfwPollEvents();
}

bool Window::KeyHold(int keycode)
{
	if (keycode < 0 || keycode >= _MOUSE_BUTTONS)
		return false;
	return KEY::keys[keycode];
}
bool Window::KeyCliked(int keycode)
{
	if (keycode < 0 || keycode >= _MOUSE_BUTTONS)
		return false;
	return KEY::keys[keycode] && KEY::frames[keycode] == KEY::current;
}

bool Window::MouseKeyHold(int button)
{
	int index = _MOUSE_BUTTONS + button;
	return KEY::keys[index];
}
bool Window::MouseKeyClicked(int button)
{
	int index = _MOUSE_BUTTONS + button;
	return KEY::keys[index] && KEY::frames[index] == KEY::current;
}

void Window::ToggleMouseLocked()
{
	KEY::cursor_locked = !KEY::cursor_locked;

	glfwSetInputMode(_window, GLFW_CURSOR, KEY::cursor_locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}