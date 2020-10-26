#pragma once
#include "Scene.h"
class LGAScene : public Scene
{
public:
	LGAScene() {}
	LGAScene(int w,int h) {}
	void init();
	void update(double);
	void render(double);
	~LGAScene();
};

