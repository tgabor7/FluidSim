#include "LBGK.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "Maths.h"
#include <iostream>

__device__ __host__
void HSVtoRGB(float& r, float& g, float& b, float h, float s, float v)
{
	int i;
	float f, p, q, t;
	if (s == 0) {
		// achromatic (grey)
		r = g = b = v;
		return;
	}
	h /= 60;			// sector 0 to 5
	i = (int)h;
	f = h - i;			// factorial part of h
	p = v * (1 - s);
	q = v * (1 - s * f);
	t = v * (1 - s * (1 - f));
	switch (i) {
	case 0:
		r = v;
		g = t;
		b = p;
		break;
	case 1:
		r = q;
		g = v;
		b = p;
		break;
	case 2:
		r = p;
		g = v;
		b = t;
		break;
	case 3:
		r = p;
		g = q;
		b = v;
		break;
	case 4:
		r = t;
		g = p;
		b = v;
		break;
	default:		// case 5:
		r = v;
		g = p;
		b = q;
		break;
	}
}

__device__ __host__
double meq(double rho, double u, double v, int k)
{
	double x;
	switch (k)
	{
	case 0: {x = rho; break; }
	case 1: {x = rho * (-2 + 3 * (u * u + v * v)); break; }
	case 2: {x = rho * (1 - 3 * (u * u + v * v)); break; }
	case 3: {x = rho * u; break; }
	case 4: {x = -rho * u; break; }
	case 5: {x = rho * v; break; }
	case 6: {x = -rho * v; break; }
	case 7: {x = rho * (u * u - v * v); break; }
	case 8: {x = rho * u * v; break; }
	default: x = 0;
	}
	return x;
}
__device__ __host__
vector3 LBGK::convertToColor(float f)
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
__global__ 
void dataKernel(double* ux, double* uy,char* data,bool* solid, double max)
{
	
	int threadsPerBlock = blockDim.x * blockDim.y;

	int threadNumInBlock = threadIdx.x + blockDim.x * threadIdx.y;

	int blockNumInGrid = blockIdx.x + gridDim.x * blockIdx.y;

	int i = blockNumInGrid * threadsPerBlock + threadNumInBlock;

	double length = sqrt(ux[i] * ux[i] + uy[i] * uy[i]);
	float cx = 255;
	float cy = 255;
	float cz = 255;


	if (!solid[i]) {
		HSVtoRGB(cz, cy, cx, (length / max) * 360, 0.95, 0.95);
		cx *= 255;
		cy *= 255;
		cz *= 255;
	}
	data[i * 4] = cx;
	data[i * 4 + 1] = cy;
	data[i * 4 + 2] = cz;
	data[i * 4 + 3] = 255;
}
__global__
void collideMRTKernel(double* f, double* fpost, double* ux, double* uy, double* rho, int* cx, int* cy, double* w, int Q, double* s, double* D){
	int threadsPerBlock = blockDim.x * blockDim.y;

	int threadNumInBlock = threadIdx.x + blockDim.x * threadIdx.y;

	int blockNumInGrid = blockIdx.x + gridDim.x * blockIdx.y;

	int i = blockNumInGrid * threadsPerBlock + threadNumInBlock;

	double MEQ;
	double m[9];

	
	m[0] = f[i * Q + 0] + f[i * Q + 1] + f[i * Q + 2] + f[i * Q + 3]
		+ f[i * Q + 4] + f[i * Q + 5] + f[i * Q + 6] + f[i * Q + 7]
		+ f[i * Q + 8];
	m[1] = -4 * f[i * Q + 0] - f[i * Q + 1] - f[i * Q + 2] - f[i * Q + 3]
		- f[i * Q + 4] + 2 * (f[i * Q + 5] + f[i * Q + 6] + f[i * Q + 7]
			+ f[i * Q + 8]);
	m[2] = 4 * f[i * Q + 0] - 2 * (f[i * Q + 1] + f[i * Q + 2] + f[i * Q + 3]
		+ f[i * Q + 4]) + f[i * Q + 5] + f[i * Q + 6] + f[i * Q + 7]
		+ f[i * Q + 8];
	m[3] = f[i * Q + 1] - f[i * Q + 3] + f[i * Q + 5] - f[i * Q + 6]
		- f[i * Q + 7] + f[i * Q + 8];
	m[4] = -2 * (f[i * Q + 1] - f[i * Q + 3]) + f[i * Q + 5] - f[i * Q + 6]
		- f[i * Q + 7] + f[i * Q + 8];
	m[5] = f[i * Q + 2] - f[i * Q + 4] + f[i * Q + 5] + f[i * Q + 6]
		- f[i * Q + 7] - f[i * Q + 8];
	m[6] = -2 * (f[i * Q + 2] - f[i * Q + 4]) + f[i * Q + 5] + f[i * Q + 6]
		- f[i * Q + 7] - f[i * Q + 8];
	m[7] = f[i * Q + 1] - f[i * Q + 2] + f[i * Q + 3] - f[i * Q + 4];
	m[8] = f[i * Q + 5] - f[i * Q + 6] + f[i * Q + 7] - f[i * Q + 8];
	// Relaxation:
	for (int k = 0; k < Q; k++)
	{
		// the equilibrium moment:
		MEQ = meq(rho[i], ux[i], uy[i], k);
		m[k] = m[k] - s[k] * (m[k] - MEQ); // relaxation
		m[k] /= D[k]; // rescaling
	}
	// Transforming back to the velocity space:
	fpost[i * Q + 0] = m[0] - 4 * (m[1] - m[2]);
	fpost[i * Q + 1] = m[0] - m[1] - 2 * (m[2] + m[4]) + m[3] + m[7];
	fpost[i * Q + 2] = m[0] - m[1] - 2 * (m[2] + m[6]) + m[5] - m[7];
	fpost[i * Q + 3] = m[0] - m[1] - 2 * (m[2] - m[4]) - m[3] + m[7];
	fpost[i * Q + 4] = m[0] - m[1] - 2 * (m[2] - m[6]) - m[5] - m[7];
	fpost[i * Q + 5] = m[0] + m[1] + m[1] + m[2] + m[3] + m[4] + m[5]
		+ m[6] + m[8];
	fpost[i * Q + 6] = m[0] + m[1] + m[1] + m[2] - m[3] - m[4] + m[5]
		+ m[6] - m[8];
	fpost[i * Q + 7] = m[0] + m[1] + m[1] + m[2] - m[3] - m[4] - m[5]
		- m[6] + m[8];
	fpost[i * Q + 8] = m[0] + m[1] + m[1] + m[2] + m[3] + m[4] - m[5]
		- m[6] - m[8];
}
__device__ __host__
double feq(double rho, double u, double v, int k, int* cx, int* cy, double* w, double deltat, double deltax)
{
	double cu, U2;
	double c = deltax / deltat;
	cu = (cx[k] * u + cy[k] * v) / c;
	U2 = (u * u + v * v) / (c*c);
	return w[k] * rho * (1.0 + 3.0 * cu + 4.5 * cu * cu - 1.5 * U2);
}

__global__
void fluidKernel(double* f, double* rho, double* ux, double* uy, int Q)
{
	int threadsPerBlock = blockDim.x * blockDim.y;

	int threadNumInBlock = threadIdx.x + blockDim.x * threadIdx.y;

	int blockNumInGrid = blockIdx.x + gridDim.x * blockIdx.y;

	int i = blockNumInGrid * threadsPerBlock + threadNumInBlock;

	rho[i] = f[i * Q] + f[i * Q + 1] + f[i * Q + 2] + f[i * Q + 3]
		+ f[i * Q + 4] + f[i * Q + 5] + f[i * Q + 6] + f[i * Q + 7] +
		f[i * Q + 8];
	ux[i] = (f[i * Q + 1] + f[i * Q + 5] + f[i * Q + 8] - f[i * Q + 3] -
		f[i * Q + 6] - f[i * Q + 7]) / rho[i];
	uy[i] = (f[i * Q + 5] + f[i * Q + 6] + f[i * Q + 2] - f[i * Q + 7] -
		f[i * Q + 8] - f[i * Q + 4]) / rho[i];

}
__global__
void checkKernel(double* f, int Q)
{
	int i = blockDim.x * blockIdx.x + threadIdx.x;

	for (int k = 0; k < 9; k++) {
		f[i * 9 + k] = 200.0;
	}
}
__global__
void streamKernel(double* f, double* f_post, bool* solid, int width, int height, int* cx, int* cy, int* rc, double* w, double* rho, double* uwx, double* uwy, int Q)
{
	int threadsPerBlock = blockDim.x * blockDim.y;

	int threadNumInBlock = threadIdx.x + blockDim.x * threadIdx.y;

	int blockNumInGrid = blockIdx.x + gridDim.x * blockIdx.y;

	int l = blockNumInGrid * threadsPerBlock + threadNumInBlock;
	int jd, id;

	int i = l % width;
	int j = (int)(l / width);


	for (int k = 0; k < Q; k++) {
		jd = j - cy[k];
		id = i - cx[k];
		if (jd >= 0 && jd <= height - 1 && id >= 0 && id <= width - 1) {
			if (!solid[jd * width + id]) {
				f[(j * width + i) * Q + k] = f_post[(jd * width + id) * Q + k];;
			}
			else {
				f[l * Q + k] = f_post[(jd * width + id) * Q + rc[k]] + 6 * w[k] * rho[l] *
					(cx[k] * uwx[jd * width + id] + cy[k] * uwy[jd * width + id]);
			}
		}
	}
}

__global__
void collideKernel(double* f, double* fpost, double* ux, double* uy, double* rho, int* cx, int* cy, double* w, int Q, double tau, int width, int height, double deltat, double deltax)
{
	int threadsPerBlock = blockDim.x * blockDim.y;

	int threadNumInBlock = threadIdx.x + blockDim.x * threadIdx.y;

	int blockNumInGrid = blockIdx.x + gridDim.x * blockIdx.y;

	int i = blockNumInGrid * threadsPerBlock + threadNumInBlock;

	for (int k = 0; k < Q; k++) {
		double FEQ = feq(rho[i], ux[i], uy[i], k, cx, cy, w, deltat, deltax);
		fpost[i * Q + k] = f[i * Q + k] - (f[i * Q + k] - FEQ) / tau;
	}
}

void LBGK::setSolid(int x, int y, bool b)
{
	if (x * width + y > width * height || x * y < 0) return;
	solid[(height-x) * width + y] = b;
}
void LBGK::update()
{
	if (err < 10) {
		//collideMRT();
		collide();
		stream();
		calcFluid();
		//err = Err();
	}
	
}
LBGK::LBGK(int width, int height)
{
	this->width = width;
	this->height = height;
	Q = 9;
	rho0 = 1.0;
	L = height + 1;
	uw = 0.1;
	Re = 800.0;
	
	ux0 = .1;
	uy0 = 0;
	f = new double[width * height * Q];
	fpost = new double[width * height * Q];
	solid = new bool[width * height];
	cx = new int[Q];
	cy = new int[Q];
	rc = new int[Q];
	uwx = new double[width * height];
	uwy = new double[width * height];
	rho = new double[width * height];
	ux = new double[width * height];
	uy = new double[width * height];
	w = new double[Q];
	D = new double[Q];
	s = new double[Q];
	u0 = new double[width * height];
	v0 = new double[width * height];
	data = new char[width * height * 4];
	cudaMalloc(&cudaF, width * height * Q * sizeof(double));
	cudaMalloc(&cudaFPost, width * height * Q * sizeof(double));
	cudaMalloc(&cudaUx, width * height * sizeof(double));
	cudaMalloc(&cudaUy, width * height * sizeof(double));
	cudaMalloc(&cudaUwx, width * height * sizeof(double));
	cudaMalloc(&cudaUwy, width * height * sizeof(double));
	cudaMalloc(&cudaRho, width * height * sizeof(double));
	cudaMalloc(&cudaW, Q * sizeof(double));
	cudaMalloc(&cudaCx, Q * sizeof(int));
	cudaMalloc(&cudaCy, Q * sizeof(int));
	cudaMalloc(&cudaRc, Q * sizeof(int));
	cudaMalloc(&cudas, Q * sizeof(double));
	cudaMalloc(&cudaD, Q * sizeof(double));
	cudaMalloc(&cudaSolid, width * height * sizeof(bool));
	cudaMalloc(&cudaData, width * height * 4 * sizeof(char));
	init();
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			solid[i * width + j] = 0;
			uwx[i * width + j] = 0;
			uwy[i * width + j] = 0;


			if (i == 1 || i == height - 2) {
				solid[i * width + j] = true;
			}
			/*for (int i = -20; i < 20; i++) {
				for (int j = -20; j < 20; j++) {
					if (sqrt(i * i + j * j) <= 5) {
						solid[(50 + i) * width + (j + 50)] = true;
					}
				}
			}*/
			/*for (int i = -20; i < 20; i++) {
				for (int j = -20; j < 20; j++) {
					if (sqrt(i * i + j * j) <= 15) {
						solid[((i + 40) * width) + (j + 150)] = true;
					}
				}
			}*/
			/*if (solid[i * width + j] == true) {
				rho[i * width + j] = 0;
				ux[i * width + j] = 0;
				uy[i * width + j] = 0;
				for (int k = 0; k < Q; k++) {
					f[(i * width + j) * Q + k] = 0;
				}
				continue;
			}*/
			rho[i * width + j] = rho0;
			ux[i * width + j] = ux0;
			uy[i * width + j] = uy0;
			for (int k = 0; k < Q; k++) {
				f[(i * width + j) * Q + k] = feq(rho[i * width + j], ux[i * width + j], uy[i * width + j], k, cx, cy, w, deltat, deltax);
			}
		}
	}
}
double LBGK::Err()
{
	int j, i;
	double e1, e2;
	e1 = e2 = 0.0;
	for (int i = 0; i < width * height; i++) {
		e1 += sqrt((ux[i] - u0[i]) * (ux[i] - u0[i])) + (uy[i] - v0[i]) * (uy[i] - v0[i]);
		e2 += sqrt((ux[i] * ux[i]) + (uy[i] * uy[i]));
		u0[i] = ux[i];
		v0[i] = uy[i];
	}
	return e1 / e2;
}

void LBGK::init()
{
	err = 1.0;
	

	
	uw = 1;

	deltat = 1;
	deltax = 1;
	tau = 3 * L * uw / Re + 0.5;

	//deltat = 2;
	//deltax = 1;
	//viscosity = .02;
	//tau = 3 * viscosity * (deltat / (deltax * deltax)) + 0.5;
	
	
	s[7] = s[8] = 1.0 / tau; s[0] = s[3] = s[5] = 0.0; s[4] = s[6] = 8 * (2 -
		s[7]) / (8 - s[7]); s[1] = 1.6; s[2] = 1.8;
	rc[0] = 0; rc[1] = 3; rc[2] = 4; rc[3] = 1; rc[4] = 2; rc[5] = 7; rc[6] = 8; rc[7] = 5; rc[8] = 6;

	D[0] = 9;
	D[1] = 36;
	D[2] = 36;
	D[3] = 6;
	D[4] = 12;
	D[5] = 6;
	D[6] = 12;
	D[7] = 4;
	D[8] = 4;

	w[0] = 4.0 / 9;
	w[1] = 1.0 / 9;
	w[2] = 1.0 / 9;
	w[3] = 1.0 / 9;
	w[4] = 1.0 / 9;
	w[5] = 1.0 / 36;
	w[6] = 1.0 / 36;
	w[7] = 1.0 / 36;
	w[8] = 1.0 / 36;

	cx[0] = 0;
	cy[0] = 0;
	cx[1] = 1;
	cy[1] = 0;
	cx[2] = 0;
	cy[2] = 1;
	cx[3] = -1;
	cy[3] = 0;
	cx[4] = 0;
	cy[4] = -1;
	cx[5] = 1;
	cy[5] = 1;
	cx[6] = -1;
	cy[6] = 1;
	cx[7] = -1;
	cy[7] = -1;
	cx[8] = 1;
	cy[8] = -1;

	cudaMemcpy(cudaW, w, Q * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaCx, cx, Q * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaCy, cy, Q * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaRc, rc, Q * sizeof(int), cudaMemcpyHostToDevice);


	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			//solid[i * width + j] = 0;
			uwx[i * width + j] = 0;
			uwy[i * width + j] = 0;


			if (i == 1 || i == height - 2) {
				solid[i * width + j] = true;
			}
			/*for (int i = -20; i < 20; i++) {
				for (int j = -20; j < 20; j++) {
					if (sqrt(i * i + j * j) <= 5) {
						solid[(50 + i) * width + (j + 50)] = true;
					}
				}
			}*/
			/*for (int i = -20; i < 20; i++) {
				for (int j = -20; j < 20; j++) {
					if (sqrt(i * i + j * j) <= 15) {
						solid[((i + 40) * width) + (j + 150)] = true;
					}
				}
			}*/
			/*if (solid[i * width + j] == true) {
				rho[i * width + j] = 0;
				ux[i * width + j] = 0;
				uy[i * width + j] = 0;
				for (int k = 0; k < Q; k++) {
					f[(i * width + j) * Q + k] = 0;
				}
				continue;
			}*/
			rho[i * width + j] = rho0;
			ux[i * width + j] = ux0;
			uy[i * width + j] = uy0;
			for (int k = 0; k < Q; k++) {
				f[(i * width + j) * Q + k] = feq(rho[i * width + j], ux[i*width+j], uy[i * width + j], k, cx, cy, w, deltat, deltax);
			}
		}
	}

	cudaMemcpy(cudaF, f, width * height * Q * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaUx, ux, width * height * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaUy, uy, width * height * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaRho, rho, width * height * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaSolid, solid, width * height * sizeof(bool), cudaMemcpyHostToDevice);

}
__device__ __host__
double LBGK::getMax() {
	double m = 0;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			double length = sqrt(ux[i * width + j] * ux[i * width + j] + uy[i * width + j] * uy[i * width + j]);
			if (length > m) m = length;
		}
	}
	return m;
}
char* LBGK::getData()
{
	/*std::vector<char> data;
	double m = getMax();
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			double length = sqrt(ux[i * width + j] * ux[i * width + j] + uy[i * width + j] * uy[i * width + j]);
			vector3 color(255);

			if (!solid[i * width + j]) {
				color = convertToColor((length / m) * 255 * 2);
			}
			data.push_back(color.x);
			data.push_back(color.y);
			data.push_back(color.z);
			data.push_back(255);
		}
	}*/
	max = getMax() * 1.5;
	
	cudaMemcpy(cudaSolid, solid, width * height * sizeof(bool), cudaMemcpyHostToDevice);

	cudaMemcpy(ux, cudaUx, width * height * sizeof(double), cudaMemcpyDeviceToHost);
	cudaMemcpy(uy, cudaUy, width * height * sizeof(double), cudaMemcpyDeviceToHost);
	cudaMemcpy(rho, cudaRho, width * height * sizeof(double), cudaMemcpyDeviceToHost);

	cudaMemcpy(cudaUx, ux, width * height * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaUy, uy, width * height * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaSolid, solid, width * height * sizeof(bool), cudaMemcpyHostToDevice);
	cudaMemcpy(f, cudaF, width * height * Q * sizeof(double), cudaMemcpyDeviceToHost);

	//void dataKernel(double* ux, double* uy, char* data, bool* solid, double max)

	dataKernel << <width, height >> > (cudaUx, cudaUy, cudaData, cudaSolid, max);

	cudaMemcpy(data, cudaData, width * height * 4 * sizeof(char), cudaMemcpyDeviceToHost);

	

	return data;
}
void LBGK::collideMRT()
{

	
	cudaMemcpy(cudaF, f, width * height * Q * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaFPost, fpost, width * height * Q * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaUx, ux, width * height * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaUy, uy, width * height * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaRho, rho, width * height * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudas, s, Q * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaD, D, Q * sizeof(double), cudaMemcpyHostToDevice);
	

	//void collideMRT(double* f, double* fpost, double* ux, double* uy, double* rho, int* cx, int* cy, double* w,int Q, double tau, double *m, double MEQ, double* s, double* D)
	//collideMRT << <width, height >> > (cudaF, cudaFPost, cudaUx, cudaUy, cudaRho, cudaCx, cudaCy, cudaW, Q, tau,cudaM, MEQ,cudas, cudaD);
	collideMRTKernel << <width,	height >> > (cudaF, cudaFPost, cudaUx, cudaUy, cudaRho, cudaCx, cudaCy, cudaW, Q, cudas, cudaD);

	cudaMemcpy(fpost, cudaFPost, width * height * Q * sizeof(double), cudaMemcpyDeviceToHost);

	
}
void LBGK::collide()
{

	/*cudaMemcpy(cudaF, f, width * height * Q * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaFPost, fpost, width * height * Q * sizeof(double), cudaMemcpyHostToDevice);*/
	/*cudaMemcpy(cudaUx, ux, width * height * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaUy, uy, width * height * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaRho, rho, width * height * sizeof(double), cudaMemcpyHostToDevice);*/
	
	collideKernel << <width, height >> > (cudaF, cudaFPost, cudaUx, cudaUy, cudaRho, cudaCx, cudaCy, cudaW, Q, tau, width, height, deltat, deltax);

	//cudaMemcpy(fpost, cudaFPost, width * height * Q * sizeof(double), cudaMemcpyDeviceToHost);

}
void LBGK::stream()
{
	/*cudaMemcpy(cudaF, f, width * height * Q * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaFPost, fpost, width * height * Q * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaUx, ux, width * height * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaUy, uy, width * height * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaUwx, uwx, width * height * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaUwy, uwy, width * height * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaRho, rho, width * height * sizeof(double), cudaMemcpyHostToDevice);*/


	//streamKernel(double* f, double* f_post, bool* solid, int width, int height, int* cx, int* cy, int* rc, double* w, double* rho, double* uwx, double* uwy, int Q)

	streamKernel << <width, height >> > (cudaF, cudaFPost, cudaSolid, width, height, cudaCx, cudaCy, cudaRc, cudaW, cudaRho, cudaUwx, cudaUwy, Q);


}
void LBGK::calcFluid()
{
	/*cudaMemcpy(cudaF, f, width * height * Q * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaUx, ux, width * height * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaUy, uy, width * height * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaRho, rho, width * height * sizeof(double), cudaMemcpyHostToDevice);*/

	fluidKernel << <width, height >> > (cudaF, cudaRho, cudaUx, cudaUy, Q);

	/*cudaMemcpy(ux, cudaUx, width * height * sizeof(double), cudaMemcpyDeviceToHost);
	cudaMemcpy(uy, cudaUy, width * height * sizeof(double), cudaMemcpyDeviceToHost);
	cudaMemcpy(rho, cudaRho, width * height * sizeof(double), cudaMemcpyDeviceToHost);*/

}
double LBGK::getF(int x, int y, int k)
{
	if (x < 0 || y < 0 || x >= height || y >= width)  return 0;

	return f[((x)*width + y)*Q + k];
}
double LBGK::getUx(int x, int y)
{
	if (x < 0 || y < 0 || x >= height || y >= width)  return 0;
	return ux[(x) * width + y];
}
double LBGK::getUy(int x, int y)
{
	if (x < 0 || y < 0 || x >= height || y >= width)  return 0;

	return uy[(x) * width + y];
}
void LBGK::setUx(int x, int y, double v)
{
	ux[x * width + y] = v;
}
void LBGK::setUy(int x, int y, double v)
{
	uy[x * width + y] = v;
}

LBGK::~LBGK()
{
	delete f;
	delete fpost;
	delete uwx;
	delete uwy;
	delete ux;
	delete uy;
	delete cx;
	delete cy;
	delete rho;
	delete w;
	delete rc;
	delete D;
	delete s;
	delete u0;
	delete v0;
	delete data;
	delete solid;

	cudaFree(cudaF);
	cudaFree(cudaFPost);
	cudaFree(cudaUx);
	cudaFree(cudaUy);
	cudaFree(cudaUwx);
	cudaFree(cudaUwy);
	cudaFree(cudaRho);
	cudaFree(cudaW);
	cudaFree(cudaCx);
	cudaFree(cudaCy);
	cudaFree(cudaRc);
	cudaFree(cudaSolid);
	cudaFree(cudas);
	cudaFree(cudaD);

}