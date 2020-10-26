#include "LGAScene.h"
#include "FHPScene.h"
#include "FHPCudaScene.h"
#include "LBMScene.h"
#include "LBGKScene.h"
#include <cassert>
#include <iostream>

using namespace std;

int main()
{
	int i;
	Scene *s = nullptr;
	cout << "Choose a scene(0: HPP, 1: FHP, 2: LBGK): ";
	cin >> i;
	switch (i) {
		case(0):
			s = new LGAScene(500,500);
			break;
		case(1):
			s = new FHPScene;
			break;
		case(2):
			s = new LBGKScene(800, 200);
			break;
		default:
			return 0;
	}
	cout << "\nPress 'A' to start simulation\n";
	switch (s->run()) {
		case(SceneState::ERROR):
			assert("Couldn't create scene!");
			break;
		case(SceneState::FINISHED):
			cout << endl << "Program terminated without errors!" << endl;
			break;
	}
	delete s;

	return 0;
}