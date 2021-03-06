#pragma once

#include "matrix.h"
#include "thread_pool.h"

void multiplyMatrices(
	PerfComparison::Matrix<double> const& first,
	PerfComparison::Matrix<double> const& second,
	PerfComparison::Matrix<double>& result,
	BlackBox::TestingThreadPool<void>* testingThreadPool = nullptr
);
