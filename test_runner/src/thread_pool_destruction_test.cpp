#define TEST_RUNNER_ENVIRONMENT

#include "thread_pool_destruction_test.h"
#include "thread_pool.h"
#include <iostream>

namespace TestCase
{

ThreadPoolDestructionTest::ThreadPoolDestructionTest()
	: m_testingObject(nullptr)
{
}

void ThreadPoolDestructionTest::run()
{
	std::cout << "Thread pool destruction test started:" << std::endl;

	try
	{
		m_testingObject = new thread_pool_type;

		std::cout << "Thread pool successful create " << m_testingObject->m_threadsNumber << " threads" << std::endl;

		for (std::size_t id = 0; id < m_testingObject->m_threadsNumber; ++id)
		{
			std::cout << "Trying to complete thread #" << id << "..." << std::endl;

			m_testingObject->completeThread(id);

			if (m_testingObject->m_threads[id].joinable())
			{
				m_testingObject->m_threads[id].join();

				std::cout << "Thread #" << id << " successful destroyed!" << std::endl;
			}
		}

		delete m_testingObject;
	}
	catch (std::exception const& e)
	{
		std::cout << "Test failed!" << std::endl;
		std::cout << e.what() << std::endl;
	}

	std::cout << std::endl << std::endl;
}

}