#include "LBGKScene.h"
#include "RenderSystem.h"
#include "Texture.h"

using namespace std;
using namespace keys;

LBGKScene::LBGKScene()
{

}
void LBGKScene::init()
{
	renderer = new RenderSystem();
	resolutionScale = 1.0;

	//create clear texture
	vector<char> data;
	for (int i = 0; i < width* resolutionScale; i++) {
		for (int j = 0; j < height* resolutionScale; j++) {
			data.push_back(255);
			data.push_back(120);
			data.push_back(0);
			data.push_back(255);
		}
	}
	texture = new Texture(&data[0], width*resolutionScale, height*resolutionScale);
	grid = new LBGK(width*resolutionScale, height*resolutionScale);

}
void LBGKScene::render(double d)
{
	if (sim)grid->update();

	texture->update(&grid->getData()[0]);
	renderer->render(texture);
}
void LBGKScene::update(double d)
{
	if (input->isKeyPressed(KEY_A)) {
		sim = true;
	}
	renderer->moveCamera(input->getMouseXf(MOUSE_BUTTON_LEFT) / 1000, input->getMouseYf(MOUSE_BUTTON_LEFT) / 1000);
	renderer->zoom(input->scroll);
	input->update();
}
LBGKScene::~LBGKScene()
{
	delete texture;
	delete renderer;
	delete grid;
}