#pragma once
#include <GLFW/glfw3.h>
#include <cassert>

class Window
{
public:
	Window();
	Window(const char* title, const int &width, const int &height, const bool &fullscreen, const bool &vsync);
	void swapBuffers();
	void resize(const int& width, const int& height, const bool& fullscreen);
	bool shouldClose();
	GLFWwindow* getWindowPointer() const;
	~Window();
private:
	GLFWwindow* windowPointer;
	char* title;
	bool fullscreen;
};

