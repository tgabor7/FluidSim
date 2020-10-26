#pragma once
#include "Input.h"
#include "Window.h"

float Input::getMouseYf(int key) {
	// calculate mouse displacement from event

	oldResultY = resultY;
	resultY = mouse.y;

	if (isMouseKeyDown(key)) {
		return resultY - oldResultY;
	}

	return 0;
}
float Input::getMouseXf(int key) {
	// calculate mouse displacement from event

	oldResultX = resultX;
	resultX = mouse.x;

	if (isMouseKeyDown(key)) {
		return resultX - oldResultX;
	}

	return 0;
}
static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	glfwGetCursorPos(window, &xpos, &ypos);
	Input::mouse.x = (float)xpos;
	Input::mouse.y = (float)ypos;
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Input::scroll = yoffset;
}
vector2 Input::mouse = vector2();
double Input::scroll = 0;
Input::Input(Window* window, int WIDTH, int HEIGHT) {
	this->win = window->getWindowPointer();
	Input::mouse = vector2();
	this->keys = new bool[GLFW_KEY_LAST];

	this->mouseKeys = new bool[GLFW_MOUSE_BUTTON_LAST];
	for (int i = 0; i < GLFW_KEY_LAST; i++) {
		keys[i] = false;
	}
	for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++) {
		mouseKeys[i] = false;
	}
	glfwSetCursorPosCallback(win, cursor_pos_callback);

	glfwSetScrollCallback(win, scroll_callback);
	if (glfwGetMouseButton(win, key) == GL_FALSE) {
		reset();
	}

}

void Input::reset() {
	resultX = mouse.x;
	resultY = mouse.y;
	oldResultY = 0;
	oldResultX = 0;
}
void Input::update() {
	Input::scroll = 0;

	for (int i = 0; i < GLFW_KEY_LAST; i++) {
		keys[i] = isKeyDown(i);
	}
	for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++) {
		mouseKeys[i] = isMouseKeyDown(i);
	}

}
bool Input::isKeyDown(int key) {
	if (glfwGetKey(win, key) == GL_TRUE) {

		return true;

	}
	else {
		return false;
	}
}
bool Input::isKeyPressed(int key) {
	return isKeyDown(key) && !keys[key];
}
bool Input::isMouseKeyDown(int key) {
	if (glfwGetMouseButton(win, key) == GL_TRUE) {

		return true;

	}
	else {
		return false;
	}

}
bool Input::isMouseKeyPressed(int key)
{
	return isMouseKeyDown(key) && !mouseKeys[key];
}
bool Input::isJoystickKeyDown(int key) {
	int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
	int count;
	const unsigned char* name = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);
	bool res = name[key] == GLFW_PRESS;
	return res;
}
vector2 Input::getJoystickAxis() {
	int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
	int count;
	const float* name = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
	return vector2(name[0], -name[1]);
}
vector2 Input::getMouse() {
	return vector2(Input::mouse.x - WIDTH / 2, Input::mouse.y - HEIGHT / 2);
}


Input::~Input() {

}