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
	const size_t n = 4096;

	PerfComparison::Matrix<double> matrixA{ n, n };
	PerfComparison::Matrix<double> matrixB{ n, n };
	PerfComparison::Matrix<double> result{ matrixA.rows(), matrixB.columns() };
	PerfComparison::Matrix<double> etalon{ matrixA.rows(), matrixB.columns() };

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

	for (size_t i = 0; i < etalon.rows(); ++i)
	{
		for (size_t j = 0; j < etalon.columns(); ++j)
		{
			etalon[i][j] = n;
		}
	}

	auto start = std::chrono::high_resolution_clock::now();

	multiplyMatrices(matrixA, matrixB, result);

	auto end = std::chrono::high_resolution_clock::now();

	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";

	if (!std::memcmp(etalon.get(), result.get(), n * n))
	{
		std::cout << "Result is valid!" << std::endl;
	}
	else
	{
		std::cout << "Result is invalid!" << std::endl;
	}

	Helpers::waitInput();
}