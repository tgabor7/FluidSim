#pragma once
#include "Scene.h"
class RenderSystem;
class Texture;
class FHPGrid;
class FHPGridCuda;
class FHPScene : public Scene
{
public:
	FHPScene() {}
	void init();
	void update(double);
	void render(double);
	~FHPScene();
private:
	RenderSystem* renderer;
	Texture* texture;
	FHPGrid* grid;
	FHPGridCuda* cudaGrid;
	bool sim = false;
};

