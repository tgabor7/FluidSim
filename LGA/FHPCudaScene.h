#pragma once
#include "Scene.h"
class RenderSystem;
class Texture;
class FHPGrid;
class FHPGridCuda;
class FHPCudaScene : public Scene
{
public:
	FHPCudaScene() {}
	void init();
	void update(double);
	void render(double);
	~FHPCudaScene();
private:
	RenderSystem* renderer;
	Texture* texture;
	FHPGrid* grid;
	FHPGridCuda* cudaGrid;
	bool sim = false;
};

