#define DLLAPI_EXPORT

#include "helpers.h"
#include "matrix.h"
#include "cpu_export_api.h"
#include "gpu_kernel.h"
#include <thread>
#include <chrono>
#include <iostream>

int main()
{
	const size_t n = 4;

	PerfComparison::Matrix<double> matrixA{ n, n };
	PerfComparison::Matrix<double> matrixB{ n, n };
	PerfComparison::Matrix<double> result{ matrixA.rows(), matrixB.columns() };

	for (size_t i = 0; i < n; ++i)
	{
		for (size_t j = 0; j < n; ++j)
		{
			std::cout << result[i][j] << " ";
		}
		std::cout << std::endl;
	}

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

	std::cout << std::endl;
	for (size_t i = 0; i < n; ++i)
	{

		for (size_t j = 0; j < n; ++j)
		{
			std::cout << result[i][j] << " ";
		}
		std::cout << std::endl;
	}

	Helpers::waitInput();
}