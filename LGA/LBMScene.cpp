#include "LBMScene.h"
#include <iostream>
#include "RenderSystem.h"
#include "Texture.h"
#include <vector>
#include <random>
#include "LBM.h"
#include "LBE.h"

using namespace std;
using namespace keys;



void LBMScene::init()
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
	grid = new LBM(width, height);
	lbegrid = new LBE;
	lbegrid->init();

}

void LBMScene::update(double delta)
{
	if (sim)lbegrid->update();

	if (input->isKeyPressed(KEY_A)) {
		sim = true;
	}
	renderer->moveCamera(input->getMouseXf(MOUSE_BUTTON_LEFT) / 1000, input->getMouseYf(MOUSE_BUTTON_LEFT) / 1000);
	renderer->zoom(input->scroll);
	//grid->handleObstacles();
	//grid->handleBoundCollision();
	input->update();
}

void LBMScene::render(double delta)
{

	texture->update(&lbegrid->getData()[0]);
	renderer->render(texture);
}
LBMScene::~LBMScene()
{
	delete texture;
	delete renderer;
	delete grid;
	delete lbegrid;
}

