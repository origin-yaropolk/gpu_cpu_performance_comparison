#pragma once

#include "itest_case.h"
#include "pimpl_helper.h"
#include "matrix.h"
#include <cstddef>
#include <mutex>
#include <memory>

namespace BlackBox
{

template <typename ReturnT>
class TestingThreadPool;

}

namespace TestCase
{

class CorrectnessCpuCalculationsTest : public ITestCase
{
public:
	CorrectnessCpuCalculationsTest();

	virtual bool run() override;
	virtual std::string name() override;

private:
	using thread_pool_type = BlackBox::TestingThreadPool<void>;

	PimplHelper<thread_pool_type> m_testingObject;

	PerfComparison::Matrix<double> m_firstMatrix;
	PerfComparison::Matrix<double> m_secondMatrix;
	PerfComparison::Matrix<double> m_resultMatrix;
	PerfComparison::Matrix<double> m_etalonResultMatrix;

	constexpr static int s_squareMatrixSize = 512;
};

}