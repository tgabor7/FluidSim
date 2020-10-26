#include "HPPGrid.h"
#include <random>
#include <iostream>

HPPGrid::HPPGrid()
{
	
}
void HPPGrid::collide()
{
	//fn = resetFunction(fn);

	for (int i = 1; i < width - 1; i++) {
		for (int j = 1; j < height - 1; j++) {
			for (int k = 0; k < 4; k++) {
				int c = f[i][j][(k + 1) % 4] * f[i][j][(k + 3) % 4] * (1 - f[i][j][k]) * (1 - f[i][j][(k + 2) % 4]) -
					(1 - f[i][j][(k + 1) % 4]) * (1 - f[i][j][(k + 3) % 4]) * f[i][j][k] * f[i][j][(k + 2) % 4];
				fn[i][j][k] = f[i][j][k] + c;
			}
		}
	}
}
void HPPGrid::handleBoundCollision()
{
	fnn = resetFunction(fnn);
	for (int i = 1; i < width - 1; i++) {
		for (int j = 1; j < height - 1; j++) {
			for (int k = 0; k < 4; k++) {
				if (i < 2 || i > width-3 || j > height-3 || j < 2) {
					fnn[i][j][(k+3)%4] = f[i][j][k];
				}
				else {
					fnn[i][j][k] = f[i][j][k];
				}
			}
		}
	}
	copy(f, fnn);
}
void HPPGrid::handleObstacles(Obstacle o)
{
	//fnn = resetFunction(fnn);

	for (int i = 1; i < width - 1; i++) {
		for (int j = 1; j < height - 1; j++) {
				if (contains(o, i, j)) {
					for (int k = 0; k < 4; k++) {
						fnn[i][j][(k + 2) % 4] = f[i][j][k];
					}
				}
				else {
					for (int k = 0; k < 4; k++) {
						fnn[i][j][k] = f[i][j][k];
					}
				}
			}
	}
	copy(f, fnn);

}
bool HPPGrid::contains(Obstacle o, int x, int y)
{
	if (x < o.pos.x || x > o.pos.x + o.dimensions.x ||
		y < o.pos.y || y > o.pos.y + o.dimensions.y) {
		return false;
	}
	return true;
}
void HPPGrid::addObstacle(vector2 p, vector2 d)
{
	Obstacle o;
	o.pos = p;
	o.dimensions = d;
	this->obstacles.push_back(o);
}
void HPPGrid::stream()
{
	//f = resetFunction(f);
	for (int i = 1; i < width-1; i++) {
		for (int j = 1; j < height-1; j++) {
			for (int k = 0; k < 4; k++) {
				vector2 e = velocities[k];
				f[(i+(int)e.x)%width][(j+(int)e.y)%height][k] = fn[i][j][k];
			}
		}
	}
}
void HPPGrid::update()
{
	collide();
	stream();
	for (int i = 0; i < obstacles.size(); i++) {
		handleObstacles(obstacles[i]);
	}
}
HPPGrid::HPPGrid(const int& width, const int& height)
{
	srand(0);
	this->width = width;
	this->height = height;

	f = initFunction(f);
	fn = initFunction(fn);
	fnn = initFunction(fnn);

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (i > 20 && i < 180 && j > 20 && j < 180) {
				f[i][j][2] = 1;
			}
		}
	}
	/*f[21][100][0] = 1;
	f[100][21][1] = 1;*/
}
void HPPGrid::copy(bool*** m, bool*** n)
{
	for (int i = 1; i < width - 1; i++) {
		for (int j = 1; j < height - 1; j++) {
			for (int k = 0; k < 4; k++) {
				m[i][j][k] = n[i][j][k];
			}
		}
	}
}
bool*** HPPGrid::initFunction(bool*** f)
{
	f = new bool** [width];
	for (int i = 0; i < width; i++) {
		f[i] = new bool* [height];
		for (int j = 0; j < height; j++) {
			f[i][j] = new bool[4];
			for (int k = 0; k < 4; k++) {
				f[i][j][k] = 0;
			}
		}
	}
	return f;
}
bool*** HPPGrid::resetFunction(bool*** f)
{
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			for (int k = 0; k < 4; k++) {
				f[i][j][k] = 0;
			}
		}
	}
	return f;
}

std::vector<char> HPPGrid::getData()
{
	std::vector<char> data;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			int density = 0;
			for (int k = 0; k < 4; k++) {
				if (f[i][j][k] == 1) density++;
			}
			data.push_back(density*63);
			data.push_back(0);
			data.push_back(0);
			data.push_back(255);
		}
	}
	for (int i = 0; i < obstacles.size(); i++) {
		for (int j = obstacles[i].pos.x; j < obstacles[i].pos.x + obstacles[i].dimensions.x; j++) {
			for (int k = obstacles[i].pos.y; k < obstacles[i].pos.y + obstacles[i].dimensions.y; k++) {
				data[j * width * 4 + (k * 4)] = 255;
				data[j * width * 4 + (k * 4) + 1] = 255;
				data[j * width * 4 + (k * 4) + 2] = 255;
				data[j * width * 4 + (k * 4) + 3] = 255;
			}
		}
	}
	return data;
}