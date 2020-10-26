#pragma once
#define GLEW_STATIC
#include "GL/glew.h"
#include "Input.h"

class Window;

enum SceneState {
	INITIALISED,
	RUNNING,
	ERROR,
	FINISHED
};

class Scene
{
public:
	Scene() {}
	Scene(int, int);
	int run();
	virtual void update(double) = 0;
	virtual void init() = 0;
	virtual void render(double) = 0;
	~Scene() {}
private:
	bool running;
	double getTime();
	double unprocessed = 0;
	double frame_time = 0;
	double frame_cap = 1.0 / 60.0;
	double delta = 0;
	int frames = 0;
	double passed = 0;
	double lastFrameTime = 0;
	double time;
	Window *window;
protected:
	Input* input;
	int width = 500;
	int height = 500;
	double resolutionScale;
};


