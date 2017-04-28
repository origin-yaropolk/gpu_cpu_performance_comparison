#define DLLAPI_EXPORT

#include "helpers.h"
#include "matrix.h"
#include "cpu_export_api.h"
#include "thread_pool.h"
#include "gpu_kernel.h"
#include <thread>
#include <chrono>
#include <iostream>

int main()
{
	const size_t n = 128;

	PerfComparison::Matrix<double> matrixA{ n, n };
	PerfComparison::Matrix<double> matrixB{ n, n };
	PerfComparison::Matrix<double> result{ matrixA.rows(), matrixB.columns() };

	for (size_t i = 0; i < matrixA.rows(); ++i)
	{
		for (size_t j = 0; j < matrixA.columns(); ++j)
		{
			matrixA[i][j] = 1;
		}
	}

	for (size_t i = 0; i < matrixB.rows(); ++i)
	{
		for (size_t j = 0; j < matrixB.columns(); ++j)
		{
			matrixB[i][j] = 1;
		}
	}

	auto start = std::chrono::high_resolution_clock::now();

	multiply_matrices(matrixA, matrixB, result);

	auto end = std::chrono::high_resolution_clock::now();

	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms";

	Helpers::waitInput();
}