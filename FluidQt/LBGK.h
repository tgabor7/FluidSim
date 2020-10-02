#pragma once
#include <vector>
#include "Maths.h"

class LBGK
{
public:
	LBGK();
	void update();
	std::vector<char> getData();
	double Re, tau;
	LBGK(int, int);
	void init();
	~LBGK();
private:
	int width;
	int height;
	void stream();
	void collide();
	void calcFluid();
	vector3 convertToColor(float);
	double getMax();
	int Q;
	int L;
	double uw;
	double rho0;
	double ux0;
	double uy0;
	double* f, * fpost, * rho, * ux, * uy, * uwx, * uwy, * w;
	bool* solid;
	int* cx, * cy;
	int* rc; //reversed velocity indices
};

