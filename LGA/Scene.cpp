#include "Scene.h"
#include "Window.h"
#include "Input.h"
#include <chrono>
#include <ctime>
#include <iostream>

#define WIDTH 500
#define HEIGHT 500
Scene::Scene(int w, int h)
{
	width = w;
	height = h;
}
int Scene::run()
{
	resolutionScale = 1.0;
	glfwInit();
	window = new Window("Fluid simulation", width, height, false, false);
	
	running = true;

	glewInit();
	input = new Input(window, width, height);
	time = getTime();
	frame_cap = 1.0 / 600.0;
	unprocessed = 0;
	init();

	while (running && !window->shouldClose()) {
		bool can_render = false;
		double time_2 = getTime();
		passed = time_2 - time;
		unprocessed += passed;
		frame_time += passed;

		time = time_2;

		while (unprocessed >= frame_cap) {
			delta = (getTime() - lastFrameTime);
			//if (delta > 100) delta = 0;
			lastFrameTime = getTime();
			unprocessed -= frame_cap;
			can_render = true;

			update(delta);

			if (frame_time >= 1.0) {

				frame_time = 0;

				std::cout << frames << std::endl;
				frames = 0;
			}
		}
		if (can_render) {
			glClearColor(.3f, .3f, .3f, 1);
			glEnable(GL_DEPTH_TEST);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_MULTISAMPLE);
			
			render(delta);
			window->swapBuffers();
			glfwPollEvents();

			frames++;
		}

	}

	delete window;
	glfwTerminate();

	return FINISHED;
}
double Scene::getTime()
{
	return (double)std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count() / (double)1000000000L;
}