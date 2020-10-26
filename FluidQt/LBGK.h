#pragma once
#include <vector>
#include "Maths.h"

class LBGK
{
public:
	LBGK();
	void update();
	char* getData();
	double Re, tau;
	LBGK(int, int);
	void setSolid(int, int, bool);
	bool* solid;
	double getUx(int, int);
	double getUy(int, int);
	double getF(int, int, int);
	void setUx(int, int, double);
	void setUy(int, int, double);
	void init();
	double ux0;
	double uy0;
	double err;
	double max;

	~LBGK();
private:
	double viscosity;
	double deltax;
	double deltat;

	double* cudaF = nullptr;
	double* cudaFPost = nullptr;
	double* cudaUx = nullptr;
	double* cudaUy = nullptr;
	double* cudaUwx = nullptr;
	double* cudaUwy = nullptr;
	double* cudaRho = nullptr;
	double* cudaW = nullptr;
	double* cudas = nullptr;
	double* cudaD = nullptr;
	int* cudaCx = nullptr;
	int* cudaCy = nullptr;
	int* cudaRc = nullptr;
	bool* cudaSolid = nullptr;

	int width;
	int height;
	char* cudaData = nullptr;
	void stream();
	void collide();
	void collideMRT();
	void calcFluid();
	char* data;
	vector3 convertToColor(float);
	double getMax();
	int Q;
	int L;
	double uw;
	double rho0;
	double* u0;
	double* v0;
	double Err();
	double* D, *s;
	int* cx, * cy;
	double* f, * fpost, * rho, * ux, * uy, * uwx, * uwy, * w;

	int* rc; //reversed velocity indices
};

