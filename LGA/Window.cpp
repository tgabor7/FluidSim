#include "Window.h"

Window::Window() {}
Window::Window(const char* title, const int& width, const int& height, const bool& fullscreen, const bool& vsync)
{
	this->fullscreen = fullscreen;
	Window::title = (char*)title;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	windowPointer = glfwCreateWindow(width, height, title, fullscreen ? glfwGetPrimaryMonitor() : 0, 0);

	if (windowPointer == NULL)
		assert("Couldn't create Window");

	if (!fullscreen) {

		GLFWvidmode vid = *glfwGetVideoMode(glfwGetPrimaryMonitor());
		glfwSetWindowPos(windowPointer, (vid.width - width) / 2, (vid.height - height) / 2);
		glfwShowWindow(windowPointer);
	}
	glfwMakeContextCurrent(windowPointer);
	glfwSwapInterval(vsync);
}
void Window::swapBuffers()
{
	glfwSwapBuffers(windowPointer);
}
bool Window::shouldClose()
{
	return glfwWindowShouldClose(windowPointer);
}

void Window::resize(const int& width, const int& height, const bool& fullscreen)
{
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	if (fullscreen) {
		glfwSetWindowMonitor(windowPointer, monitor, 0, 0, width, height, 0);
	}
	else {
		glfwSetWindowMonitor(windowPointer, nullptr, 0, 0, width, height, 0);
		glfwSetWindowPos(windowPointer, (mode->width - width) / 2, (mode->height - height) / 2);
	}
}

GLFWwindow* Window::getWindowPointer() const
{
	return windowPointer;
}
Window::~Window()
{
	glfwDestroyWindow(windowPointer);
}