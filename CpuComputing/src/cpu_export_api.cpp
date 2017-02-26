#define DLLAPI_EXPORT

#include "cpu_export_api.h"
#include "cpu_helper_api.h"
#include "thread_pool.h"
#include <vector>
#include <iostream>
#include <future>
#include <cassert>

CPU_API_EXPORT bool multiply_matrices(
	PerfComparison::Matrix<double> const& first,
	PerfComparison::Matrix<double> const& second,
	PerfComparison::Matrix<double>& result)
{
	std::vector<Helpers::ThreadSpecificDataHolder> taskPackets =
		Helpers::distributeTasks(first, second, result);

	black_box::thread_pool<void>& threadPool =
		black_box::thread_pool<void>::instance();

	std::vector<black_box::thread_pool<void>::result_type> futures;

	for (size_t i = 0; i < taskPackets.size(); ++i)
	{
		futures.push_back(threadPool.add_task(Helpers::processPartJob, taskPackets[i]));
	}

	processPartJob(taskPackets[taskPackets.size() - 1]);

	for (auto const& future : futures)
	{
		future.wait();
	}

	return true;
}