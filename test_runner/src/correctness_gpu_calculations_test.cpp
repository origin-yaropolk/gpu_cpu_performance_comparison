#include "correctness_gpu_calculations_test.h"


#include <iostream>

namespace TestCase
{

	CorrectnessGpuCalculationsTest::CorrectnessGpuCalculationsTest()
		: m_firstMatrix(s_squareMatrixSize, s_squareMatrixSize)
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


	bool CorrectnessGpuCalculationsTest::run()
	{
		float placeHolder;
		std::cout << "Performing GPU calculations..." << std::endl;

		GPU_test(m_firstMatrix, m_secondMatrix, m_resultMatrix, placeHolder);

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

	std::string CorrectnessGpuCalculationsTest::name()
	{
		return "CorrectnessGpuCalculationsTest";
	}

}