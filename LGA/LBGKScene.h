#pragma once
#include "Scene.h"
#include "LBGK.h"

class RenderSystem;
class Texture;
class LBGKScene : public Scene
{
public:
	LBGKScene();
	LBGKScene(int w, int h) { width = w; height = h; }
	void init();
	void update(double);
	void render(double);
	~LBGKScene();
private:
	LBGK* grid;
	RenderSystem* renderer;
	Texture* texture;
	bool sim = false;
};

