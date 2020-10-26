#pragma once
#include <GLFW\glfw3.h>
#include "Maths.h"

namespace keys {
	static int KEY_ESCAPE = GLFW_KEY_ESCAPE;

	static int KEY_UP = GLFW_KEY_UP;
	static int KEY_DOWN = GLFW_KEY_DOWN;
	static int KEY_LEFT = GLFW_KEY_LEFT;
	static int KEY_RIGHT = GLFW_KEY_RIGHT;
	static int KEY_SPACE = GLFW_KEY_SPACE;
	static int KEY_ENTER = GLFW_KEY_ENTER;
	static int KEY_A = GLFW_KEY_A;
	static int KEY_B = GLFW_KEY_B;
	static int KEY_C = GLFW_KEY_C;
	static int KEY_D = GLFW_KEY_D;
	static int KEY_E = GLFW_KEY_E;
	static int KEY_F = GLFW_KEY_F;
	static int KEY_G = GLFW_KEY_G;
	static int KEY_H = GLFW_KEY_H;
	static int KEY_I = GLFW_KEY_I;
	static int KEY_J = GLFW_KEY_J;
	static int KEY_K = GLFW_KEY_K;
	static int KEY_L = GLFW_KEY_L;
	static int KEY_M = GLFW_KEY_M;
	static int KEY_N = GLFW_KEY_N;
	static int KEY_O = GLFW_KEY_O;
	static int KEY_P = GLFW_KEY_P;
	static int KEY_Q = GLFW_KEY_Q;
	static int KEY_R = GLFW_KEY_R;
	static int KEY_S = GLFW_KEY_S;
	static int KEY_T = GLFW_KEY_T;
	static int KEY_U = GLFW_KEY_U;
	static int KEY_V = GLFW_KEY_V;
	static int KEY_W = GLFW_KEY_W;
	static int KEY_X = GLFW_KEY_X;
	static int KEY_Y = GLFW_KEY_Z;
	static int KEY_Z = GLFW_KEY_Y;
	static int KEY_0 = GLFW_KEY_KP_0;
	static int KEY_1 = GLFW_KEY_KP_1;
	static int KEY_2 = GLFW_KEY_KP_2;
	static int KEY_3 = GLFW_KEY_KP_3;
	static int KEY_4 = GLFW_KEY_KP_4;
	static int KEY_5 = GLFW_KEY_KP_5;
	static int KEY_6 = GLFW_KEY_KP_6;
	static int KEY_7 = GLFW_KEY_KP_7;
	static int KEY_8 = GLFW_KEY_KP_8;
	static int KEY_9 = GLFW_KEY_KP_9;
	static int KEY_ANY = GLFW_KEY_LAST;
	static int KEY_SHIFT = GLFW_KEY_LEFT_SHIFT;
	static int KEY_DELETE = GLFW_KEY_DELETE;

	static int MOUSE_BUTTON_RIGHT = GLFW_MOUSE_BUTTON_RIGHT;
	static int MOUSE_BUTTON_LEFT = GLFW_MOUSE_BUTTON_LEFT;
	static int MOUSE_BUTTON_MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE;

	static int JOYSTICK_KEY_1 = 0;
	static int JOYSTICK_KEY_2 = 1;
	static int JOYSTICK_KEY_3 = 2;
	static int JOYSTICK_KEY_4 = 3;
	static int JOYSTICK_KEY_5 = 4;
	static int JOYSTICK_KEY_6 = 5;
	static int JOYSTICK_KEY_7 = 6;
	static int JOYSTICK_KEY_8 = 7;

}
class Window;
class Input {


public:
	Input(Window* window, int WIDTH, int HEIGHT);
	GLFWwindow* win;
	static vector2 mouse;
	static double scroll;
	bool* keys;
	bool* mouseKeys;
	bool* joystickKeys;
	int key;
	void reset();
	float resultX;
	float resultY;
	float oldResultX;
	float oldResultY;
	void update();
	bool isKeyDown(int key);
	bool isKeyPressed(int key);

	bool isMouseKeyDown(int key);
	bool isMouseKeyPressed(int key);
	bool isJoystickKeyDown(int key);
	int WIDTH = WIDTH;
	int HEIGHT = HEIGHT;

	float getMouseXf(int key);
	float getMouseYf(int key);
	vector2 getMouse();
	vector2 getJoystickAxis();
	~Input();

};


