#include "cpu_export_api.h"
#include "cpu_helper_api.h"
#include "thread_pool.h"
#include <vector>
#include <iostream>
#include <future>
#include <cassert>

bool multiplyMatrices(
	PerfComparison::Matrix<double> const& first,
	PerfComparison::Matrix<double> const& second,
	PerfComparison::Matrix<double>& result,
	BlackBox::TestingThreadPool<void>* testingThreadPool)
{
	std::vector<Helpers::ThreadSpecificDataHolder> taskPackets =
		Helpers::distributeTasks(first, second, result, testingThreadPool);

	BlackBox::ThreadPool<void>* threadPool = nullptr;

	if (testingThreadPool == nullptr)
	{
		threadPool = &BlackBox::ThreadPool<void>::instance();
	}
	else
	{
		threadPool = testingThreadPool;
	}

	for (size_t i = 0; i < taskPackets.size(); ++i)
	{
		threadPool->addTask(Helpers::processPartJob, taskPackets[i]);
	}

	processPartJob(taskPackets[taskPackets.size() - 1]);

	threadPool->waitAll();

	if (testingThreadPool)
	{
		testingThreadPool->completeAllThreads();
	}

	return true;
}