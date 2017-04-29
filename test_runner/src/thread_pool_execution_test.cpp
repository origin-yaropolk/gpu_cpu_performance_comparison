#define TEST_RUNNER_ENVIRONMENT

#include "thread_pool_execution_test.h"
#include "thread_pool.h"
#include <iostream>

namespace TestCase
{

ThreadPoolExecutionTest::ThreadPoolExecutionTest()
	: m_testingObject(&thread_pool_type::instance())
{
}

void ThreadPoolExecutionTest::run()
{
	std::cout << "Add " << s_tasksCounter << " tasks for execution" << std::endl;

	for (std::size_t i = 0; i < s_tasksCounter; ++i)
	{
		m_testingObject->addTask(testFunction);
	}

	std::cout << "Waiting all threads..."<< std::endl;

	m_testingObject->waitAll();

	if (s_executionCounter == s_tasksCounter)
	{
		std::cout << "All tasks successful executed!" << std::endl;
	}

	m_testingObject->completeAllThreads();
}

std::string ThreadPoolExecutionTest::name()
{
	return std::string("ThreadPoolExecutionTest");
}

void ThreadPoolExecutionTest::testFunction()
{
	std::lock_guard<std::mutex> locker{ s_mutex };
	++s_executionCounter;
}

/****************************  Data for test ********************************/

std::size_t ThreadPoolExecutionTest::s_executionCounter;

std::mutex ThreadPoolExecutionTest::s_mutex;

const std::size_t ThreadPoolExecutionTest::s_tasksCounter = 5000;

}