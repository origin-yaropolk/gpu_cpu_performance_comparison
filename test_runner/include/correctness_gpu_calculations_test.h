#pragma once

#include "itest_case.h"
#include "matrix.h"
#include "gpu_kernel.h"

namespace TestCase
{
	class CorrectnessGpuCalculationsTest : public ITestCase
	{
	public:
		CorrectnessGpuCalculationsTest();

		virtual bool run() override;
		virtual std::string name() override;

	private:
	
		PerfComparison::Matrix<float> m_firstMatrix;
		PerfComparison::Matrix<float> m_secondMatrix;
		PerfComparison::Matrix<float> m_resultMatrix;
		PerfComparison::Matrix<float> m_etalonResultMatrix;

		constexpr static int s_squareMatrixSize = 128;
	};
}