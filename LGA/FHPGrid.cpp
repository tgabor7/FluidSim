#include "FHPGrid.h"
#include <random>
#include <iostream>

FHPGrid::FHPGrid()
{

}
void FHPGrid::collide()
{
	//fn = resetFunction(fn);
	for (int i = 1; i < width - 1; i++) {
		for (int j = 1; j < height - 1; j++) {
			int r = rand() % 2;

			for (int k = 0; k < 6; k++) {

				int c = f[i][j][(k + 1) % 6] * f[i][j][(k + 3) % 6] * f[i][j][(k + 5) % 6] * (1-f[i][j][k]) * (1-f[i][j][(k + 2) % 6]) * (1-f[i][j][(k + 4) % 6]) - //elsõ blokk :(
					(1-f[i][j][(k + 1) % 6]) * (1-f[i][j][(k + 3) % 6]) * (1-f[i][j][(k + 5) % 6]) * f[i][j][k] * f[i][j][(k + 2) % 6] * f[i][j][(k + 4) % 6] + //második blokk q.q
					r * f[i][j][(k + 1) % 6] * f[i][j][(k + 4) % 6] * (1-f[i][j][k]) * (1-f[i][j][(k + 2) % 6]) * (1-f[i][j][(k + 3) % 6]) * (1-f[i][j][(k + 5) % 6]) +//félút
					(1 - r) * f[i][j][(k + 2) % 6] * f[i][j][(k + 5) % 6] * (1-f[i][j][k]) * (1-f[i][j][(k + 1) % 6]) * (1-f[i][j][(k + 3) % 6]) * (1-f[i][j][(k + 4) % 6]) -//utolsó jön
					f[i][j][k] * f[i][j][(k + 3) % 6] * (1-f[i][j][(k + 1) % 6]) * (1-f[i][j][(k + 2) % 6]) * (1-f[i][j][(k + 4) % 6]) * (1-f[i][j][(k + 5) % 6]); // vége
				
				//if (f[i][j][k] == 1) std::cout << "c: " <<  c << "r: " << r << "\n";
				
				fn[i][j][k] = f[i][j][k] + c;
			}
		}
	}
}
void FHPGrid::handleBoundCollision()
{
	fnn = resetFunction(fnn);
	for (int i = 1; i < width - 1; i++) {
		for (int j = 1; j < height - 1; j++) {
			if (solid[i][j] == 1) {
				for (int k = 0; k < 6; k++) {
					fnn[i][j][(k + 5) % 6] = f[i][j][k];
				}
			}
			else {
				for (int k = 0; k < 6; k++) {
					fnn[i][j][k] = f[i][j][k];
				}
			}
		}
	}
	copy(f, fnn);
}
void FHPGrid::handleObstacles(Obstacle o)
{
	fnn = resetFunction(fnn);

	for (int i = 1; i < width - 1; i++) {
		for (int j = 1; j < height - 1; j++) {
			if (contains(o, i, j)) {
				for (int k = 0; k < 6; k++) {
					fnn[i][j][(k + 5) % 6] = f[i][j][k];
				}
			}
			else {
				for (int k = 0; k < 6; k++) {
					fnn[i][j][k] = f[i][j][k];
				}
			}
		}
	}
	copy(f, fnn);

}
bool FHPGrid::contains(Obstacle o, int x, int y)
{
	if (x < o.pos.x || x > o.pos.x + o.dimensions.x ||
		y < o.pos.y || y > o.pos.y + o.dimensions.y) {
		return false;
	}
	return true;
}
void FHPGrid::addObstacle(vector2 p, vector2 d)
{
	Obstacle o;
	o.pos = p;
	o.dimensions = d;
	this->obstacles.push_back(o);


	for (int i = p.x; i < p.x + d.x; i++) {
		for (int j = p.y; j < p.y + d.y; j++) {
			solid[i][j] = 1;
		}
	}
}
void FHPGrid::stream()
{
	//f = resetFunction(f);
	for (int i = 1; i < width - 1; i++) {
		for (int j = 1; j < height - 1; j++) {
			for (int k = 0; k < 6; k++) {
				vector2 e = velocities[k];
				
				f[(i + (int)e.x) % width][(j + (int)e.y) % height][k] = fn[i][j][k];
			}
		}
	}
}
void FHPGrid::update()
{
	collide();
	stream();
	/*for (int i = 0; i < obstacles.size(); i++) {
		handleObstacles(obstacles[i]);
	}*/
	handleBoundCollision();
}
FHPGrid::FHPGrid(const int& width, const int& height)
{
	srand(0);
	this->width = width;
	this->height = height;

	velocities[0] = vector2(1, -1);
	velocities[1] = vector2(1, 0);
	velocities[2] = vector2(1, 1);
	velocities[3] = vector2(-1, 1);
	velocities[4] = vector2(-1, 0);
	velocities[5] = vector2(-1, -1);
	f = initFunction(f);
	fn = initFunction(fn);
	fnn = initFunction(fnn);
	solid = new int* [width];
	for (int j = 0; j < width; j++) {
		solid[j] = new int[height];
		for (int i = 0; i < height; i++) {
			solid[j][i] = 0;
		}
	}
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (i > 20 && i < 180 && j > 20 && j < 180) {
				f[i][j][1] = 1;
			}
		}
	}
	
}
void FHPGrid::copy(bool*** m, bool*** n)
{
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			for (int k = 0; k < 6; k++) {
				m[i][j][k] = n[i][j][k];
			}
		}
	}
}
bool*** FHPGrid::initFunction(bool*** f)
{
	f = new bool** [width];
	for (int i = 0; i < width; i++) {
		f[i] = new bool* [height];
		for (int j = 0; j < height; j++) {
			f[i][j] = new bool[6];
			for (int k = 0; k < 6; k++) {
				f[i][j][k] = 0;
			}
		}
	}
	return f;
}
bool*** FHPGrid::resetFunction(bool*** f)
{
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			for (int k = 0; k < 6; k++) {
				f[i][j][k] = 0;
			}
		}
	}
	return f;
}

std::vector<char> FHPGrid::getData()
{
	std::vector<char> data;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			int density = 0;
			for (int k = 0; k < 6; k++) {
				if (f[j][i][k] == 1) density++;
			}
			data.push_back(density * 255);
			data.push_back(0);
			data.push_back(0);
			data.push_back(255);
		}
	}
	for (int i = 0; i < obstacles.size(); i++) {
		for (int k = obstacles[i].pos.x; k < obstacles[i].pos.x + obstacles[i].dimensions.x; k++) {
			for (int j = obstacles[i].pos.y; j < obstacles[i].pos.y + obstacles[i].dimensions.y; j++) {
				data[j * width * 4 + (k * 4)] = 255;
				data[j * width * 4 + (k * 4) + 1] = 255;
				data[j * width * 4 + (k * 4) + 2] = 255;
				data[j * width * 4 + (k * 4) + 3] = 255;
			}
		}
	}
	return data;
}