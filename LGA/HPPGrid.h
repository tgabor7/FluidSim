#pragma once
#include <vector>
#include "Maths.h"

class Obstacle {
public:
	vector2 pos;
	vector2 dimensions;
};
class HPPGrid
{
public:
	HPPGrid();
	HPPGrid(const int &width, const int &height);
	
	void addObstacle(vector2, vector2);
	void update();
	std::vector<char> getData();
private:
	void stream();
	void collide();
	void handleBoundCollision();
	void handleObstacles(Obstacle);

	std::vector<Obstacle> obstacles;
	int width;
	int height;
	bool*** initFunction(bool*** f);
	bool*** resetFunction(bool*** f);
	void copy(bool*** m, bool*** n);
	bool contains(Obstacle, int, int);
	bool*** f;
	bool*** fn;
	bool*** fnn;
	const vector2 velocities[4] = {vector2(1,0),vector2(0,1),vector2(-1,0),vector2(0,-1)};
};

