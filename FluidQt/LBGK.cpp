#include "LBGK.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "Maths.h"
#include <iostream>

__device__ __host__
double feq(double rho, double u, double v, int k, int* cx, int* cy, double* w)
{
	double cu, U2;
	cu = cx[k] * u + cy[k] * v;
	U2 = u * u + v * v;
	return w[k] * rho * (1.0 + 3.0 * cu + 4.5 * cu * cu - 1.5 * U2);
}
__global__
void textureKernel()
{

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
void collideKernel(double* f, double* fpost, double* ux, double* uy, double* rho, int* cx, int* cy, double* w, int Q, double tau)
{
	int threadsPerBlock = blockDim.x * blockDim.y;

	int threadNumInBlock = threadIdx.x + blockDim.x * threadIdx.y;

	int blockNumInGrid = blockIdx.x + gridDim.x * blockIdx.y;

	int i = blockNumInGrid * threadsPerBlock + threadNumInBlock;


	for (int k = 0; k < Q; k++) {
		double FEQ = feq(rho[i], ux[i], uy[i], k, cx, cy, w);
		fpost[i * Q + k] = f[i * Q + k] - (f[i * Q + k] - FEQ) / tau;
	}
}


void LBGK::update()
{


	collide();

	stream();

	calcFluid();
}
LBGK::LBGK(int width, int height)
{
	this->width = width;
	this->height = height;
	Q = 9;
	rho0 = 1.0;
	L = height + 1;
	uw = 0.1;
	Re = 200.0;
	
	ux0 = 0;
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
	init();
}
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
void LBGK::init()
{
	tau = 3 * L * uw / Re + 0.5;

	rc[0] = 0; rc[1] = 3; rc[2] = 4; rc[3] = 1; rc[4] = 2; rc[5] = 7; rc[6] = 8; rc[7] = 5; rc[8] = 6;

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
			for (int i = -20; i < 20; i++) {
				for (int j = -20; j < 20; j++) {
					if (sqrt(i * i + j * j) <= 15) {
						solid[((i + 40) * width) + (j + 150)] = true;
					}
				}
			}

			rho[i * width + j] = rho0;
			ux[i * width + j] = ux0;
			uy[i * width + j] = uy0;
			for (int k = 0; k < Q; k++) {
				f[(i * width + j) * Q + k] = feq(rho[i * width + j], .1, uy[i * width + j], k, cx, cy, w);
			}
		}
	}
}
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
std::vector<char> LBGK::getData()
{
	std::vector<char> data;
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
	}
	return data;
}
void LBGK::collide()
{

	double* cudaF;
	double* cudaFPost;
	double* cudaUx;
	double* cudaUy;
	double* cudaRho;
	double* cudaW;
	int* cudaCx;
	int* cudaCy;

	cudaMalloc(&cudaF, width * height * Q * sizeof(double));
	cudaMalloc(&cudaFPost, width * height * Q * sizeof(double));
	cudaMalloc(&cudaUx, width * height * sizeof(double));
	cudaMalloc(&cudaUy, width * height * sizeof(double));
	cudaMalloc(&cudaRho, width * height * sizeof(double));
	cudaMalloc(&cudaW, Q * sizeof(double));
	cudaMalloc(&cudaCx, Q * sizeof(int));
	cudaMalloc(&cudaCy, Q * sizeof(int));

	cudaMemcpy(cudaF, f, width * height * Q * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaFPost, fpost, width * height * Q * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaUx, ux, width * height * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaUy, uy, width * height * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaRho, rho, width * height * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaW, w, Q * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaCx, cx, Q * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaCy, cy, Q * sizeof(int), cudaMemcpyHostToDevice);

	collideKernel << <width, height >> > (cudaF, cudaFPost, cudaUx, cudaUy, cudaRho, cudaCx, cudaCy, cudaW, Q, tau);

	cudaMemcpy(fpost, cudaFPost, width * height * Q * sizeof(double), cudaMemcpyDeviceToHost);

	cudaFree(cudaF);
	cudaFree(cudaFPost);
	cudaFree(cudaUx);
	cudaFree(cudaUy);
	cudaFree(cudaRho);
	cudaFree(cudaW);
	cudaFree(cudaCx);
	cudaFree(cudaCy);
	cudaError_t cudaStatus;

	cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
	}
}
void LBGK::calcFluid()
{
	double* cudaF;
	double* cudaUx;
	double* cudaUy;
	double* cudaRho;

	cudaMalloc(&cudaF, width * height * Q * sizeof(double));
	cudaMalloc(&cudaUx, width * height * sizeof(double));
	cudaMalloc(&cudaUy, width * height * sizeof(double));
	cudaMalloc(&cudaRho, width * height * sizeof(double));

	cudaMemcpy(cudaF, f, width * height * Q * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaUx, ux, width * height * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaUy, uy, width * height * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaRho, rho, width * height * sizeof(double), cudaMemcpyHostToDevice);

	fluidKernel << <width, height >> > (cudaF, cudaRho, cudaUx, cudaUy, Q);

	cudaMemcpy(ux, cudaUx, width * height * sizeof(double), cudaMemcpyDeviceToHost);
	cudaMemcpy(uy, cudaUy, width * height * sizeof(double), cudaMemcpyDeviceToHost);
	cudaMemcpy(rho, cudaRho, width * height * sizeof(double), cudaMemcpyDeviceToHost);

	cudaFree(cudaF);
	cudaFree(cudaUx);
	cudaFree(cudaUy);
	cudaFree(cudaRho);
}
void LBGK::stream()
{
	double* cudaF;
	double* cudaFPost;
	double* cudaUx;
	double* cudaUy;
	double* cudaUwx;
	double* cudaUwy;
	double* cudaRho;
	double* cudaW;
	int* cudaCx;
	int* cudaCy;
	int* cudaRc;
	bool* cudaSolid;

	cudaMalloc(&cudaF, width * height * Q * sizeof(double));
	cudaMalloc(&cudaFPost, width * height * Q * sizeof(double));
	cudaMalloc(&cudaUx, width * height * sizeof(double));
	cudaMalloc(&cudaUy, width * height * sizeof(double));
	cudaMalloc(&cudaUwx, width * height * sizeof(double));
	cudaMalloc(&cudaUwy, width * height * sizeof(double));
	cudaMalloc(&cudaRho, width * height * sizeof(double));
	cudaMalloc(&cudaSolid, width * height * sizeof(bool));
	cudaMalloc(&cudaW, Q * sizeof(double));
	cudaMalloc(&cudaCx, Q * sizeof(int));
	cudaMalloc(&cudaCy, Q * sizeof(int));
	cudaMalloc(&cudaRc, Q * sizeof(int));

	cudaMemcpy(cudaF, f, width * height * Q * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaFPost, fpost, width * height * Q * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaUx, ux, width * height * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaUy, uy, width * height * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaUwx, uwx, width * height * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaUwy, uwy, width * height * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaRho, rho, width * height * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaSolid, solid, width * height * sizeof(bool), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaW, w, Q * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaCx, cx, Q * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaCy, cy, Q * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(cudaRc, rc, Q * sizeof(int), cudaMemcpyHostToDevice);

	//streamKernel(double* f, double* f_post, bool* solid, int width, int height, int* cx, int* cy, int* rc, double* w, double* rho, double* uwx, double* uwy, int Q)

	streamKernel << <width, height >> > (cudaF, cudaFPost, cudaSolid, width, height, cudaCx, cudaCy, cudaRc, cudaW, cudaRho, cudaUwx, cudaUwy, Q);

	cudaMemcpy(f, cudaF, width * height * Q * sizeof(double), cudaMemcpyDeviceToHost);


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
}