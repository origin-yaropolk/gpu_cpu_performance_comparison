#define TEST_RUNNER_ENVIRONMENT

#include "thread_pool.h"
#include "cpu_export_api.h"
#include "correctness_cpu_calculations_test.h"
#include <iostream>

namespace TestCase
{

CorrectnessCpuCalculationsTest::CorrectnessCpuCalculationsTest()
	: m_testingObject(std::thread::hardware_concurrency())
	, m_firstMatrix(s_squareMatrixSize, s_squareMatrixSize)
	, m_secondMatrix(s_squareMatrixSize, s_squareMatrixSize)
	, m_resultMatrix(m_firstMatrix.rows(), m_secondMatrix.columns())
	, m_etalonResultMatrix(m_firstMatrix.rows(), m_secondMatrix.columns())
{
	for (size_t i = 0; i < m_firstMatrix.rows(); ++i)
	{
		for (size_t j = 0; j < m_firstMatrix.columns(); ++j)
		{
			m_firstMatrix[i][j] = 1;
		}
	}

	for (size_t i = 0; i < m_secondMatrix.rows(); ++i)
	{
		for (size_t j = 0; j < m_secondMatrix.columns(); ++j)
		{
			m_secondMatrix[i][j] = 1;
		}
	}

	for (size_t i = 0; i < m_etalonResultMatrix.rows(); ++i)
	{
		for (size_t j = 0; j < m_etalonResultMatrix.columns(); ++j)
		{
			m_etalonResultMatrix[i][j] = s_squareMatrixSize;
		}
	}
}


bool CorrectnessCpuCalculationsTest::run()
{
	std::cout << "Performing CPU calculations..." << std::endl;

	multiplyMatrices(m_firstMatrix, m_secondMatrix, m_resultMatrix, &(*m_testingObject));

	std::cout << "Calculated! Checking validness..." << std::endl;

	if (!std::memcmp(m_resultMatrix.get(), m_etalonResultMatrix.get(), s_squareMatrixSize * s_squareMatrixSize))
	{
		std::cout << "Calculations valid!" << std::endl;

		return true;
	}
	else
	{
		std::cout << "Calculations invalid!" << std::endl;

		return false;
	}
}

std::string CorrectnessCpuCalculationsTest::name()
{
	return "CorrectnessCpuCalculationsTest";
}

}