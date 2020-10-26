#include "LBM.h"
#include <math.h>
#include <iostream>

LBM::LBM(int width, int height)
{
	this->width = width;
	this->height = height;
	tau = 1;
	rhoS = 1;
	rad = 6;
	distRad = 0.003125;
	ep = 1e-6;
	g = .001;
	initVelocity();
	initFunction(f);
	initFunction(ftemp);
	initFunction(feq);
	initWeights();
	initRho();
	initSolids();
	initLatticVel();

	//buildLattice();

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			for (int k = 0; k < 9; k++) {
				f[i][j][k] = t[k] * rho[i][j];
				//if (j > 0 && j < 30 && i>10 && i < height-10 && k == 3) f[i][j][k] = .5;
			}
		}
	}
}
void LBM::initLatticVel()
{
	e[0][0] = 0;
	e[0][1] = 0;
	e[1][0] = 1;
	e[1][1] = 0;
	e[2][0] = 1;
	e[2][1] = 1;
	e[3][0] = 0;
	e[3][1] = 1;
	e[4][0] = -1;
	e[4][1] = 1;
	e[5][0] = -1;
	e[5][1] = 0;
	e[6][0] = -1;
	e[6][1] = -1;
	e[7][0] = 0;
	e[7][1] = -1;
	e[8][0] = 1;
	e[8][1] = -1;
}
void LBM::initSolids()
{
	solid = new bool* [width];
	for (int i = 0; i < width; i++) {
		solid[i] = new bool[height];
		for (int j = 0; j < height; j++) {
			solid[i][j] = 0;
			if (i < 10 || i>width - 10) solid[i][j] = 1;
			if (i > 200 && i < 210 && j>10 && j < 230) solid[i][j] = 1;
			if (i > 200 && i < 210 && j>250 && j < 500) solid[i][j] = 1;
			if (i > 80 && i < 90 && j>180 && j < 190) solid[i][j] = 1;
		}
	}
}
void LBM::initFunction(double*** &f)
{
	f = new double** [width];
	for (int i = 0; i < width; i++) {
		f[i] = new double* [height];
		for (int j = 0; j < height; j++) {
			f[i][j] = new double[9];
			for (int k = 0; k < 9; k++) {
				f[i][j][k] = 0;
			}
		}
	}
}
void LBM::calculateEqdist()
{
	for (int i = 0; i < width; i++) {
		for(int j = 0;j < height;j++){
			ueq[0] = u[i][j][0];
			ueq[1] = u[i][j][1] - tau*g;

			ux_uy2 = ueq[0] + ueq[1];
			ux_uy4 = -ueq[0] + ueq[1];
			ux_uy6 = -ueq[0] - ueq[1];
			ux_uy8 = ueq[0] - ueq[1];

			u_dot_u = pow(ueq[0], 2) + pow(ueq[1], 2);

			if (solid[i][j] == 0) {
				feq[i][j][0] = rho[i][j] * t[0] * (1 + 0 + 0 - f3 * u_dot_u);
				feq[i][j][1] = rho[i][j] * t[1] * (1 + f1*ueq[0] + f2*pow(ueq[0],2) - f3 * u_dot_u);
				feq[i][j][2] = rho[i][j] * t[2] * (1 + f1*ux_uy2 + f2*ux_uy2*ux_uy2 - f3 * u_dot_u);
				feq[i][j][3] = rho[i][j] * t[3] * (1 + f1*ueq[1] + f2*pow(ueq[1],2) - f3 * u_dot_u);
				feq[i][j][4] = rho[i][j] * t[4] * (1 + f1*ux_uy4 + f2*ux_uy4*ux_uy4 - f3 * u_dot_u);
				feq[i][j][5] = rho[i][j] * t[5] * (1 - f1*ueq[0] + f2*pow(ueq[0],2) - f3 * u_dot_u);
				feq[i][j][6] = rho[i][j] * t[6] * (1 + f1*ux_uy6 + f2*ux_uy6*ux_uy6 - f3 * u_dot_u);
				feq[i][j][7] = rho[i][j] * t[7] * (1 - f1*ueq[1] + f2*pow(ueq[1],2) - f3 * u_dot_u);
				feq[i][j][8] = rho[i][j] * t[8] * (1 + f1*ux_uy8 + f2*ux_uy8*ux_uy8 - f3 * u_dot_u);
			}
		}
	}
}
void LBM::buildLattice()
{
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (((double)rand() / (RAND_MAX)) <= distRad) {
				for (int ii = -rad; ii <= rad; ii++) {
					for (int jj = -rad; jj <= rad; jj++) {
						if (i + ii < 0)
							i_p = (width - 1) + (1 + (i + ii));
						else if (i + ii > (width - 1))
							i_p = ((i + ii)) - (width - 1) - 1;
						else
							i_p = i + ii;

						if (j + jj < 0)
							j_p = (height - 1) + (1 + (j + jj));
						else if (j + jj > (height - 1))
							j_p = ((j + jj)) - (height - 1) - 1;
						else
							j_p = j + jj;

						if (sqrt(pow(ii, 2) + pow(jj, 2)) < rad + pow(1. / rad, 2)) {
							solid[i_p][j_p] = 1;
						}
					}
				}
				solid[i][j] = 1;
			}
		}
	}
}
void LBM::calculateStreaming()
{
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			i_n = i - 1;
			i_p = i + 1;

			j_n = j - 1;
			j_p = j + 1;

			if (i_n < 0) {
				i_n = (height - 1) + (1 + i_n);
			}
			else if (i_p > (height - 1)) {
				i_p = (i_p - (height - 1)) - 1;
			}
			if (j_n < 0) {
				j_n = (width - 1) + (1 + j_n);
			}
			else if (j_p > (width - 1)) {
				j_p = (j_p - (width - 1)) - 1;
			}

			ftemp[i][j][0] = f[i][j][0];
			ftemp[i_p][j][1] = f[i][j][1];
			ftemp[i_p][j_p][2] = f[i][j][2];
			ftemp[i][j_p][3] = f[i][j][3];
			ftemp[i_n][j_p][4] = f[i][j][4];
			ftemp[i_n][j][5] = f[i][j][5];
			ftemp[i_n][j_n][6] = f[i][j][6];
			ftemp[i][j_n][7] = f[i][j][7];
			ftemp[i_p][j_n][8] = f[i][j][8];
		}
	}
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			for (int k = 0; k < 9; k++) {
				f[i][j][k] = ftemp[i][j][k];
			}
		}
	}
}
vector3 LBM::convertToColor(float f)
{
	vector3 color;
	if (f < 256) {
		color.z = 255 - f;
		color.y = f;
	}
	else if (f > 255) {
		color.x = f - 256;
		color.y = 255 - (f - 256);
	}
	return color;
}
std::vector<char> LBM::getData()
{
	std::vector<char> data;
	std::cout << sqrt((u[250][100][0] * u[250][100][0]) + (u[250][100][1] * u[250][100][1])) << "\n";
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			double length = (((u[i][j][0] * u[i][j][0]) + (u[i][j][1] * u[i][j][1])))*10000;
			
			vector3 color = convertToColor(length);
			
			if (solid[i][j]) {
				data.push_back(255);
				data.push_back(255);
				data.push_back(255);
				data.push_back(255);
			}
			else {
				//double length = rho[i][j]*123;
				data.push_back((int)color.x);
				data.push_back((int)color.y);
				data.push_back((int)color.z);
				data.push_back(255);
			}
			
		}
	}
	return data;
}
void LBM::update()
{
	calculateMacro();
	calculateEqdist();
	calculateCollision();
	calculateStreaming();
}
void LBM::calculateCollision()
{
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (solid[i][j] == 1) {

				temp = f[i][j][1];
				f[i][j][1] = f[i][j][5];
				f[i][j][5] = temp;

				temp = f[i][j][3];
				f[i][j][3] = f[i][j][7];
				f[i][j][7] = temp;

				temp = f[i][j][2];
				f[i][j][2] = f[i][j][6];
				f[i][j][6] = temp;

				temp = f[i][j][4];
				f[i][j][4] = f[i][j][8];
				f[i][j][8] = temp;
			}
			else {
				for (int k = 0; k < 9; k++) {
					f[i][j][k] = f[i][j][k] - (1.0f / tau) * (f[i][j][k] - feq[i][j][k]);
				}
			}
		}
	}
}
void LBM::calculateMacro()
{
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {

			u[i][j][0] = 0.0;
			u[i][j][1] = 0.0;
			rho[i][j] = 0.0;

			if (solid[i][j] == 0) {
				for (int k = 0; k < 9; k++) {
					rho[i][j] = rho[i][j] + f[i][j][k];
					u[i][j][0] = u[i][j][0] + e[k][0] * f[i][j][k];
					u[i][j][1] = u[i][j][1] + e[k][1] * f[i][j][k];
				}
				u[i][j][0] = u[i][j][0] / rho[i][j];
				u[i][j][1] = u[i][j][1] / rho[i][j];
			}
		}
	}
	
}
void LBM::initRho()
{
	rho = new double* [width];
	for (int i = 0; i < width; i++) {
		rho[i] = new double[height];
		for (int j = 0; j < height; j++) {
			rho[i][j] = rhoS;
		}
	}
	for (int i = 200; i < 230; i++) {
		for (int j = 200; j < 230; j++) {
			rho[i][j] = 1;
		}
	}
	/*rho[250][250] = 20;
	rho[250][251] = 20;
	rho[251][251] = 20;
	rho[251][250] = 20;*/
	
}
void LBM::initVelocity()
{
	u = new double**[width];
	u_old = new double** [width];
	for (int i = 0; i < width; i++) {
		u[i] = new double* [height];
		u_old[i] = new double*[height];
		for (int j = 0; j < height; j++) {
			u[i][j] = new double[2];
			u_old[i][j] = new double[2];
			for (int k = 0; k < 2; k++) {
				u[i][j][k] = 0;
				u_old[i][j][k] = 0;
			}
		}
	}
}
void LBM::initWeights()
{
	t = new double[9];
	t[0] = 4. / 9.;
	t[1] = 1. / 9.;
	t[2] = 1. / 36.;
	t[3] = 1. / 9.;
	t[4] = 1. / 36.;
	t[5] = 1. / 9.;
	t[6] = 1. / 36.;
	t[7] = 1. / 9.;
	t[8] = 1. / 36.;
}
void LBM::deleteFunction()
{

}
LBM::~LBM()
{
	
}