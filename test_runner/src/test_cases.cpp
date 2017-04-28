#define TEST_RUNNER_ENVORONMENT

#include "test_cases.h"
#include "thread_pool.h"

namespace TestCases
{

ThreadPoolDestructionTest::ThreadPoolDestructionTest()
	: m_testingObject(&thread_pool_type::instance())
{
}

void ThreadPoolDestructionTest::run()
{

}

void ThreadPoolDestructionTest::testFunction()
{

}

/****************************  Data for test ********************************/

std::size_t ThreadPoolDestructionTest::m_executionCounter;

std::mutex ThreadPoolDestructionTest::m_mutex;

}