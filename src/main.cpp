#define DLLAPI_EXPORT
#define ERROR_DISPLAYING

#include "helpers.h"
#include "matrix.h"
#include "cpu_export_api.h"
#include "thread_pool.h"
#include "gpu_kernel.h"
#include <thread>
#include <chrono>
#include <iostream>
#include <string>
#include <conio.h>

<<<<<<< HEAD
int main()
{
	const size_t n = 4096;

	PerfComparison::Matrix<double> matrixA{ n, n };
	PerfComparison::Matrix<double> matrixB{ n, n };
	PerfComparison::Matrix<double> result{ matrixA.rows(), matrixB.columns() };
	PerfComparison::Matrix<double> etalon{ matrixA.rows(), matrixB.columns() };
=======
using namespace std;

void FillMatrix(PerfComparison::Matrix<float>& matrix)
{
	for (size_t i = 0; i < matrix.rows(); ++i)
	{
		for (size_t j = 0; j < matrix.columns(); ++j)
		{
			matrix[i][j] = 1;
		}
	}
}
>>>>>>> d1840fc21a7068cd20a55273c19b205c9f6b9bef

void FillMatrix(PerfComparison::Matrix<double>& matrix)
{
	for (size_t i = 0; i < matrix.rows(); ++i)
	{
		for (size_t j = 0; j < matrix.columns(); ++j)
		{
			matrix[i][j] = 1;
		}
	}
}


size_t SizeMenuSelector(const string& title, const vector<string>& items)
{
	size_t selectorPos=0;
	unsigned symbol;
	while (true)
	{
		system("cls");
		std::cout << title << endl;
		for (int i = 0; i < items.size(); i++)
		{
			std::cout << "  " << ((i == selectorPos) ? "[*] " : "[ ] ");
			std::cout << items[i] << std::endl;
		}
		getch();
		symbol = getch();
		if ((symbol == 80) && (selectorPos < items.size()-1))
		{
			selectorPos++;
		}
		if ((symbol == 72) && (selectorPos > 0))
		{
			selectorPos--;
		}
		if (symbol == 13)
		{
			return selectorPos;
		}

	}
	return 0;
}

size_t CountMenuSelector(const string& title)
{
	size_t count = 1;
	system("cls");
	std::cout << title;
	cin >> count;
	return count;
}

int main()
{
	vector<string> sizeChooseMenuItems =
	{
		"32","64","128","256","512","1024","2048"
	};

	float GPUcomputingTime = 0;
	long long CPUcomputingTime = 0;
	const size_t sizesCount = 7;
	const size_t availableSizes[sizesCount] = { 32,64,128,256,512,1024,2048 };
	size_t matricesSize = availableSizes[SizeMenuSelector("Choose matrices size:", sizeChooseMenuItems)];
	size_t matricesCount = CountMenuSelector("Enter matrices count: ");
	system("cls");
	
	cout << "Parameters: \n  Matrix size = " << matricesSize << "\n  matricesCount = " << matricesCount << endl << endl;

<<<<<<< HEAD
	for (size_t i = 0; i < etalon.rows(); ++i)
	{
		for (size_t j = 0; j < etalon.columns(); ++j)
		{
			etalon[i][j] = n;
		}
	}

	auto start = std::chrono::high_resolution_clock::now();
=======
	cout << "Performing GPU calculations..." << endl;
	for (auto i = 0; i < matricesCount; i++)
	{
		PerfComparison::Matrix<float> matrixA{ matricesSize, matricesSize };
		PerfComparison::Matrix<float> matrixB{ matricesSize, matricesSize };
		PerfComparison::Matrix<float> result{ matrixA.rows(), matrixB.columns() };
		FillMatrix(matrixA);
		FillMatrix(matrixB);
		float tempTime = 0;
		GPU_test(matrixA, matrixB, result, tempTime);
		GPUcomputingTime += tempTime;
	}
>>>>>>> d1840fc21a7068cd20a55273c19b205c9f6b9bef

	cout << "  computing Time - " << GPUcomputingTime << " ms" << endl << endl;
	cout << "Performing CPU calculations..." << endl;
	for (auto i = 0; i < matricesCount; i++)
	{
		PerfComparison::Matrix<double> matrixA{ matricesSize, matricesSize };
		PerfComparison::Matrix<double> matrixB{ matricesSize, matricesSize };
		PerfComparison::Matrix<double> result{ matrixA.rows(), matrixB.columns() };
		FillMatrix(matrixA);
		FillMatrix(matrixB);

		auto start = chrono::high_resolution_clock::now();
		multiplyMatrices(matrixA, matrixB, result);
		auto end = chrono::high_resolution_clock::now();
		CPUcomputingTime += chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

	}
	cout << "  computing Time - " << CPUcomputingTime << " ms" << endl << endl;
	if (CPUcomputingTime > GPUcomputingTime)
	{
		cout << "GPU x" << CPUcomputingTime / GPUcomputingTime << " faster than CPU" << endl;
	}
	else
	{
		cout << "CPU x" << GPUcomputingTime / CPUcomputingTime << " faster than GPU" << endl;
	}
	cout << "There are about " << matricesCount * (matricesSize * matricesSize * (matricesSize + matricesSize - 1)) << " floating operations performed and " << matricesCount *( 2 * matricesSize*matricesSize * sizeof(float)) << " bytes of information processed." << endl;

<<<<<<< HEAD
	if (!std::memcmp(etalon.get(), result.get(), n * n))
	{
		std::cout << "Result is valid!" << std::endl;
	}
	else
	{
		std::cout << "Result is invalid!" << std::endl;
	}
=======
>>>>>>> d1840fc21a7068cd20a55273c19b205c9f6b9bef

	system("pause");
}