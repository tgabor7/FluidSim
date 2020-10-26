#include "LGAScene.h"
#include <iostream>
#include "RenderSystem.h"
#include "Texture.h"
#include <vector>
#include <random>
#include "HPPGrid.h"

using namespace std;
using namespace keys;

RenderSystem *renderer;
Texture* texture;
HPPGrid* grid;
bool sim = false;

void LGAScene::init()
{
	width = 500;
	height = 500;

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
	grid = new HPPGrid(width, height);
	
	//box
	grid->addObstacle(vector2(0, 0), vector2(500, 10));
	grid->addObstacle(vector2(0, 0), vector2(10, 500));
	grid->addObstacle(vector2(0, 490), vector2(500, 10));
	grid->addObstacle(vector2(490, 0), vector2(10, 500));

	//slit
	grid->addObstacle(vector2(0, 200), vector2(220, 10));
	grid->addObstacle(vector2(280, 200), vector2(220,10));
}

void LGAScene::update(double delta)
{
	if (input->isKeyPressed(KEY_A)) {
		sim = true;
	}
	//grid->handleObstacles();
	//grid->handleBoundCollision();
	input->update();
}

void LGAScene::render(double delta)
{
	if(sim)grid->update();

	texture->update(&grid->getData()[0]);
	renderer->render(texture);
}
LGAScene::~LGAScene()
{
	delete texture;
	delete renderer;
	delete grid;
}

