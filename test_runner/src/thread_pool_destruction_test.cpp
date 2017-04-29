#define TEST_RUNNER_ENVIRONMENT

#include "thread_pool_destruction_test.h"
#include "thread_pool.h"
#include <iostream>
#include <string>

namespace TestCase
{

ThreadPoolDestructionTest::ThreadPoolDestructionTest()
	: m_testingObject(nullptr)
{
}

void ThreadPoolDestructionTest::run()
{
	try
	{
		m_testingObject = new thread_pool_type;

		std::cout << "Thread pool successful create " << m_testingObject->m_threadsNumber << " threads" << std::endl;

		for (std::size_t id = 0; id < m_testingObject->m_threadsNumber; ++id)
		{
			std::cout << "Trying to complete thread #" << id << "... ";

			m_testingObject->completeThread(id);

			if (m_testingObject->m_threads[id].joinable())
			{
				m_testingObject->m_threads[id].join();

				std::cout << "Successful destroyed!" << std::endl;
			}
		}

		delete m_testingObject;
	}
	catch (std::exception const& e)
	{
		std::cout << "Test failed!" << std::endl;
		std::cout << e.what() << std::endl;
	}
}

std::string ThreadPoolDestructionTest::name()
{
	return std::string("ThreadPoolDestructionTest");
}

}