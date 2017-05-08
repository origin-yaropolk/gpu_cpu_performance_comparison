#pragma once

#define TEST_RUNNER_ENVIRONMENT

#include "itest_case.h"
#include "pimpl_helper.h"
#include "thread_pool.h"
#include <vector>
#include <mutex>
#include <condition_variable>

namespace BlackBox
{
	template <typename ReturnT>
	class TestingThreadPool;

	class IMessage;
}

namespace TestCase
{

class ThreadPoolWatchDogTest 
	: public ITestCase
	, public BlackBox::TestReceiver
{
public:
	ThreadPoolWatchDogTest();

	virtual bool run() override;
	virtual std::string name() override;
	virtual void receiveMessage(const BlackBox::IMessage& message) override;

private:
	static void testFunction();
	static const std::size_t s_tasksCounter;
	static const std::size_t s_functionTimeoutMs;

private:
	using thread_pool_type = BlackBox::TestingThreadPool<void>;

	PimplHelper<thread_pool_type> m_testingObject;
	std::vector<bool> m_restartedThreads;
	std::mutex m_mutex;
	std::condition_variable m_threadKilledCondition;
};

}