#define API_EXPORT


#include "gpu_kernel.h"
#include "matrix.h"
#include "helpers.h"

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <iostream>

namespace
{
	const size_t BLOCK_SIZE = 32;
}

struct Matrix
{
	size_t width;
	size_t height;
	size_t stride;
	float* elements;
};

__device__ float GetElement(const Matrix A, size_t row, size_t col)
{
	return A.elements[row * A.stride + col];
}

__device__ void SetElement(const Matrix A, size_t row, size_t col, float value)
{
	A.elements[row * A.stride + col] = value;
}

__device__ Matrix GetSubMatrix(Matrix A, size_t row, size_t col)
{
	Matrix Asub;
	Asub.width = BLOCK_SIZE;
	Asub.height = BLOCK_SIZE;
	Asub.stride = A.stride;
	Asub.elements = &A.elements[A.stride * BLOCK_SIZE * row + BLOCK_SIZE * col];
	return Asub;
}

__global__ void GPU_test_helper(const Matrix A, const Matrix B, Matrix C)
{
	size_t blockRow = blockIdx.y;
	size_t blockCol = blockIdx.x;

	Matrix Csub = GetSubMatrix(C, blockRow, blockCol);

	float Cvalue = 0;

	size_t row = threadIdx.y;
	size_t col = threadIdx.x;

	for (size_t m = 0; m < (A.width / BLOCK_SIZE); ++m)
	{
		Matrix Asub = GetSubMatrix(A, blockRow, m);
		Matrix Bsub = GetSubMatrix(B, m, blockCol);

		__shared__ float As[BLOCK_SIZE][BLOCK_SIZE];
		__shared__ float Bs[BLOCK_SIZE][BLOCK_SIZE];

		As[row][col] = GetElement(Asub, row, col);
		Bs[row][col] = GetElement(Bsub, row, col);
		__syncthreads();

		for (size_t e = 0; e < BLOCK_SIZE; ++e)
		{
			Cvalue += As[row][e] * Bs[e][col];
		}
		__syncthreads();
	}
	SetElement(Csub, row, col, Cvalue);

	/*int row = blockIdx.y * blockDim.y + threadIdx.y;
	int col = blockIdx.x * blockDim.x + threadIdx.x;
	C.elements[row*C.width + col] = A.elements[row * A.width + col] + B.elements[row * B.width + col];
	*/
}

__host__ void WhatAnError(cudaError_t er)
{
#ifdef ERROR_DISPLAYING

	switch (er)
	{
		//cudaSuccess
	case 0: std::cerr << "CUDA SUCCESS." << std::endl; break;
		//cudaErrorMissingConfiguration
	case 1: std::cerr << "CUDA ERROR MISSING CONFIGURATION." << std::endl; break;
		//cudaErrorMemoryAllocation
	case 2: std::cerr << "CUDA ERROR MEMORY ALLOCATION." << std::endl; break;
		//cudaErrorInitializationError
	case 3: std::cerr << "CUDA ERROR INITIALIZATION ERROR." << std::endl; break;
		//cudaErrorLaunchFailure
	case 4: std::cerr << "CUDA ERROR LAUNCH FAILURE." << std::endl; break;
		//cudaErrorPriorLaunchFailure
	case 5: std::cerr << "CUDA ERROR PRIOR LAUNCH FAILURE." << std::endl; break;
		//cudaErrorLaunchTimeout
	case 6: std::cerr << "CUDA ERROR LAUNCH TIMEOUT." << std::endl; break;
		//unknowError
	default: std::cerr << "CUDA ERROR CODE: " << er << std::endl; break;
	}
#endif // ERROR_DISPLAYING
}

__global__ void DisplayMatrixKernel(Matrix const& Mat)
{
	for (int i = 0; i < Mat.height; i++)
	{
		for (int j = 0; j < Mat.width; j++)
		{
			printf("%f", Mat.elements[i * Mat.width + j]);
		}
		printf("\n");
	}
}

void DisplayMatrix(double const* matrix, size_t size, char* name)
{
	std::cout << std::endl << name << std::endl;
	for (size_t i = 0; i < size*size; i++)
	{
		if (i % size == 0)
		{
			std::cout << std::endl;
		}
		std::cout << matrix[i] << " ";
	}

	std::cout << std::endl;
}

void GPU_test(PerfComparison::Matrix<float> const& A, PerfComparison::Matrix<float> const& B, PerfComparison::Matrix<float> &C, float& ComputingTime)
{
	cudaError_t error;
	cudaEvent_t start, stop;
	Matrix device_A, device_B, device_C;
	size_t bytesNumber;

	device_A.height = A.rows();
	device_A.width = A.columns();
	device_A.stride = A.columns();
	bytesNumber = device_A.width * device_A.height * sizeof(float);
	error = cudaMalloc(&device_A.elements, bytesNumber);
	WhatAnError(error);
	error = cudaMemcpy(device_A.elements, A.get(), bytesNumber, cudaMemcpyHostToDevice);
	WhatAnError(error);


	device_B.height = B.rows();
	device_B.width = B.columns();
	device_B.stride = B.columns();
	bytesNumber = device_B.width * device_B.height * sizeof(float);
	error = cudaMalloc(&device_B.elements, bytesNumber);
	WhatAnError(error);
	error = cudaMemcpy(device_B.elements, B.get(), bytesNumber, cudaMemcpyHostToDevice);
	WhatAnError(error);

	device_C.height = C.rows();
	device_C.width = C.columns();
	device_C.stride = C.columns();
	bytesNumber = device_C.width * device_C.height * sizeof(float);
	error = cudaMalloc(&device_C.elements, bytesNumber);
	WhatAnError(error);

	error = cudaEventCreate(&start);
	WhatAnError(error);
	error = cudaEventCreate(&stop);
	WhatAnError(error);

	dim3 threads(BLOCK_SIZE, BLOCK_SIZE);
	dim3 blocks(device_B.width / threads.x, device_A.height / threads.y);

	error = cudaEventRecord(start, 0);
	WhatAnError(error);

	/*
	std::cout << "matrix A" << std::endl;
	DisplayMatrixKernel<<<1,1>>>(device_A);
	std::cout << "matrix B" << std::endl;
	DisplayMatrixKernel<<<1,1>>>(device_B);
	*/

	GPU_test_helper <<<blocks, threads >>>(device_A, device_B, device_C);


	error = cudaThreadSynchronize();
	//error = cudaDeviceSynchronize();
	WhatAnError(error);
	error = cudaEventRecord(stop, 0);
	WhatAnError(error);
	error = cudaEventSynchronize(stop);
	WhatAnError(error);

	error = cudaEventElapsedTime(&ComputingTime, start, stop);
	WhatAnError(error);


	error = cudaMemcpy(C.get(), device_C.elements, bytesNumber, cudaMemcpyDeviceToHost);
	WhatAnError(error);


	error = cudaFree(device_A.elements);
	WhatAnError(error);
	error = cudaFree(device_B.elements);
	WhatAnError(error);
	error = cudaFree(device_C.elements);
	WhatAnError(error);
	error = cudaEventDestroy(start);
	WhatAnError(error);
	error = cudaEventDestroy(stop);
	WhatAnError(error);
}