#pragma once
#include "Scene.h"
class RenderSystem;
class Texture;
class LBM;
class LBE;
class LBMScene : public Scene
{
public:
	LBMScene() {}
	LBMScene(int w, int h) { width = w; height = h; }
	void init();
	void update(double);
	void render(double);
	~LBMScene();
private:
	RenderSystem* renderer;
	Texture* texture;
	LBM* grid;
	LBE* lbegrid;
	bool sim = false;
};

