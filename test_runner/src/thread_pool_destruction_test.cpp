#define TEST_RUNNER_ENVIRONMENT

#include "thread_pool_destruction_test.h"
#include "thread_pool.h"
#include <iostream>
#include <string>

namespace TestCase
{

ThreadPoolDestructionTest::ThreadPoolDestructionTest()
{
}

bool ThreadPoolDestructionTest::run()
{
	m_testingObject.reset(new PimplHelper<thread_pool_type>(std::thread::hardware_concurrency(), 5000));

	bool result = false;

	try
	{
		std::cout << "Thread pool successful create " << (*m_testingObject.get())->m_threadsNumber << " threads" << std::endl;

		for (std::size_t id = 0; id < (*m_testingObject.get())->m_threadsNumber; ++id)
		{
			std::cout << "Trying to complete thread #" << id << "... ";

			(*m_testingObject.get())->completeThread(id);

			if ((*m_testingObject.get())->m_threads[id].joinable())
			{
				(*m_testingObject.get())->m_threads[id].join();

				std::cout << "Successful destroyed!" << std::endl;
			}

			(*m_testingObject.get())->m_watchDogs[id].completeWatchDogThread.store(true);

			if ((*m_testingObject.get())->m_watchDogs[id].watchDogThread.joinable())
			{
				(*m_testingObject.get())->m_watchDogs[id].watchDogThread.join();
			}
		}
	}
	catch (std::exception const& e)
	{
		std::cout << "Test failed!" << std::endl;
		std::cout << e.what() << std::endl;
	}

	return true;
}

std::string ThreadPoolDestructionTest::name()
{
	return std::string("ThreadPoolDestructionTest");
}

}