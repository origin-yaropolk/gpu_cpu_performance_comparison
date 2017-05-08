#pragma once

#include "itest_case.h"
#include "pimpl_helper.h"
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

class ThreadPoolExecutionTest : public ITestCase
{
public:
	ThreadPoolExecutionTest();

	virtual bool run() override;
	virtual std::string name() override;

private:
	static void testFunction();

private:
	static std::size_t s_executionCounter;
	static std::mutex s_mutex;
	static const std::size_t s_tasksCounter;

private:
	using thread_pool_type = BlackBox::TestingThreadPool<void>;

	std::unique_ptr<PimplHelper<thread_pool_type>> m_testingObject;
};

}