#define DLLAPI_EXPORT

#include "cpu_export_api.h"
#include "cpu_helper_api.h"
#include "thread_pool.h"
#include <vector>
#include <iostream>
#include <future>
#include <cassert>

CPU_API_EXPORT bool multiplyMatrices(
	PerfComparison::Matrix<double> const& first,
	PerfComparison::Matrix<double> const& second,
	PerfComparison::Matrix<double>& result)
{
	std::vector<Helpers::ThreadSpecificDataHolder> taskPackets =
		Helpers::distributeTasks(first, second, result);

	BlackBox::ThreadPool<void>& threadPool =
		BlackBox::ThreadPool<void>::instance();

	for (size_t i = 0; i < taskPackets.size(); ++i)
	{
		threadPool.addTask(Helpers::processPartJob, taskPackets[i]);
	}

	processPartJob(taskPackets[taskPackets.size() - 1]);

	threadPool.waitAll();

	return true;
}