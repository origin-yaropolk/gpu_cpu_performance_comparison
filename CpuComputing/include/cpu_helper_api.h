#pragma once

#include "matrix.h"
#include "thread_pool.h"
#include <cstddef>

namespace Helpers
{

struct ThreadSpecificDataHolder
{
	size_t startRow;
	size_t size;

	PerfComparison::Matrix<double> const& firstSourceMatrix;
	PerfComparison::Matrix<double> const& secondSourceMatrix;
	PerfComparison::Matrix<double>& resultMatrix;
};

void processPartJob(ThreadSpecificDataHolder specific);

std::vector<ThreadSpecificDataHolder> distributeTasks(
	PerfComparison::Matrix<double> const& first,
	PerfComparison::Matrix<double> const& second,
	PerfComparison::Matrix<double>& result
);

}