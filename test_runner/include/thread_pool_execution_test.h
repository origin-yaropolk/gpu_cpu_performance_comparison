#pragma once

#include "itest_case.h"
#include <cstddef>
#include <mutex>

namespace BlackBox
{

template <typename ReturnT>
class ThreadPool;

}

namespace TestCase
{

class ThreadPoolExecutionTest : public ITestCase
{
public:
	ThreadPoolExecutionTest();

	virtual void run() override;

private:
	static void testFunction();

private:
	static std::size_t s_executionCounter;
	static std::mutex s_mutex;
	static const std::size_t s_tasksCounter;

private:
	using thread_pool_type = BlackBox::ThreadPool<void>;

	thread_pool_type* m_testingObject;
};

}