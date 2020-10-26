#pragma once
#include <vector>
#include "Maths.h"
class LBM
{
public:
	LBM();
	LBM(int, int);
	std::vector<char> getData();
	void update();
	~LBM();
private:
	void initFunction(double*** &f);
	void initWeights();
	void initRho();
	void initVelocity();
	void initSolids();
	void deleteFunction();

	void calculateMacro();
	void calculateEqdist();
	void calculateCollision();
	void calculateStreaming();
	void initLatticVel();
	void buildLattice();

	vector3 convertToColor(float f);

	bool** solid;
	double dist;
	int rad;
	double distRad;
	double ep;
	double temp;
	double*** f;
	double*** ftemp;
	double tau;
	int e[9][2];
	double rhoS;
	double** rho;
	double*** feq;
	double* t;
	double*** u;
	double*** u_old;
	double diff;
	double u_diff;
	double ueq[2];
	double g = 0.001;
	double ux_uy2, ux_uy4, ux_uy6, ux_uy8;
	double u_dot_u;
	double f1 = 3;
	double f2 = 9. / 2.;
	double f3 = 3. / 2.;

	int i_n, i_p, j_n, j_p;

	int width;
	int height;
};

