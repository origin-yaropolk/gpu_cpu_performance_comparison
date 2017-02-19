#include "cpu_helper_api.h"
#include "thread_pool.h"

namespace Helpers
{

void processPartJob(ThreadSpecificDataHolder specific)
{
	for (size_t i = 0; i < specific.size; ++i)
	{
		PerfComparison::Matrix<double>::Row const& row =
			specific.firstSourceMatrix[specific.startRow + i];

		for (size_t j = 0; j < specific.secondSourceMatrix.columns(); ++j)
		{
			PerfComparison::Matrix<double>::Column secondMatrixColumn =
				specific.secondSourceMatrix.columnAt(j);

			double value = 0;

			for (size_t k = 0; k < specific.secondSourceMatrix.rows(); ++k)
			{
				value += row[k] * secondMatrixColumn[k];
			}

			specific.resultMatrix[i][j] = value;
		}
	}
}

std::vector<ThreadSpecificDataHolder> distributeTasks(
	PerfComparison::Matrix<double> const& first,
	PerfComparison::Matrix<double> const& second,
	PerfComparison::Matrix<double>& result)
{
	const bool isMatricesValidForMultiply = first.rows() == second.columns();

	const bool isResultMatrixHasValidSize =
		result.rows() == first.rows() && result.columns() == second.columns();

	if (!isMatricesValidForMultiply || !isResultMatrixHasValidSize)
	{
		throw std::invalid_argument{ "Incompatible sizes of matrices" };
	}

	black_box::thread_pool<void>& threadPool = 
		black_box::thread_pool<void>::instance();

	// plus main thread
	size_t threadsNumber = threadPool.working_threads() + 1;

	const size_t payloadPerThread = first.rows() / threadsNumber;
	const size_t remainderPerThread = first.rows() % threadsNumber;

	std::vector<ThreadSpecificDataHolder> tasks;

	size_t startRow = 0;

	for (size_t i = 0; i < threadsNumber - 1; ++i)
	{
		tasks.push_back(ThreadSpecificDataHolder{ startRow, payloadPerThread, first, second, result });
		startRow += payloadPerThread;
	}

	tasks.push_back(ThreadSpecificDataHolder{ startRow, payloadPerThread + remainderPerThread, first, second, result });

	return tasks;
}

}
