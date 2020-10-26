#include "FHPScene.h"
#include <iostream>
#include "RenderSystem.h"
#include "Texture.h"
#include <vector>
#include <random>
#include "FHPGrid.h"

using namespace std;
using namespace keys;



void FHPScene::init()
{
	renderer = new RenderSystem();
	srand(0);


	//create clear texture
	vector<char> data;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			data.push_back(255);
			data.push_back(120);
			data.push_back(0);
			data.push_back(255);
		}
	}
	texture = new Texture(&data[0], width, height);
	grid = new FHPGrid(width, height);

	//box
	grid->addObstacle(vector2(0, 0), vector2(10, 500));
	grid->addObstacle(vector2(0, 0), vector2(500, 10));
	grid->addObstacle(vector2(490, 0), vector2(10, 500));
	grid->addObstacle(vector2(0, 490), vector2(500, 10));

	//slit
	grid->addObstacle(vector2(200, 0), vector2(10, 220));
	grid->addObstacle(vector2(200, 280), vector2(10, 220));

	/*grid->addObstacle(vector2(0, 200), vector2(220, 10));
	grid->addObstacle(vector2(280, 200), vector2(220, 10));*/
}

void FHPScene::update(double delta)
{
	if (input->isKeyPressed(KEY_A)) {
		sim = true;
	}
	renderer->moveCamera(input->getMouseXf(MOUSE_BUTTON_LEFT) / 1000, input->getMouseYf(MOUSE_BUTTON_LEFT) / 1000);
	renderer->zoom(input->scroll);
	//grid->handleObstacles();
	//grid->handleBoundCollision();
	input->update();
}

void FHPScene::render(double delta)
{
	if (sim)grid->update();

	texture->update(&grid->getData()[0]);
	renderer->render(texture);
}
FHPScene::~FHPScene()
{
	delete texture;
	delete renderer;
	delete grid;
	delete cudaGrid;
}

