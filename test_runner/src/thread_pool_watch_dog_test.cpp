#define TEST_RUNNER_ENVIRONMENT

#include "thread_pool_watch_dog_test.h"
#include "thread_pool.h"
#include <iostream>
#include <chrono>
#include <thread>

namespace TestCase
{

ThreadPoolWatchDogTest::ThreadPoolWatchDogTest()
	: m_testingObject(s_tasksCounter, s_functionTimeoutMs)
	, m_restartedThreads(s_tasksCounter, false)
{
	//m_testingObject->addReceiver(this);
}

bool ThreadPoolWatchDogTest::run()
{
	std::cout << "Start " << s_tasksCounter << " threads..." << std::endl;

	for (std::size_t i = 0; i < s_tasksCounter; ++i)
	{
		m_testingObject->addTask(testFunction);
	}

	auto countValueInVector = [this]
	{
		for (bool value : m_restartedThreads)
		{
			if (!value)
			{
				return false;
			}
		}

		return true;
	};

	std::unique_lock<std::mutex> locker(m_mutex);

	if (m_threadKilledCondition.wait_until(locker, 
		std::chrono::system_clock::now() + std::chrono::milliseconds(s_functionTimeoutMs + 500), 
		countValueInVector))
	{
		std::cout << "All threads successful killed after timeout" << std::endl;
	}
	else
	{
		std::cout << "Something was wrong! Not all threads were restarted." << std::endl;
	}

	for (std::size_t id = 0; id < m_testingObject->m_threadsNumber; ++id)
	{
		m_testingObject->completeThread(id);

		if (m_testingObject->m_threads[id].joinable())
		{
			m_testingObject->m_threads[id].join();
		}

		m_testingObject->m_watchDogs[id].completeWatchDogThread.store(true);

		if (m_testingObject->m_watchDogs[id].watchDogThread.joinable())
		{
			m_testingObject->m_watchDogs[id].watchDogThread.join();
		}
	}

	return true;
}

std::string ThreadPoolWatchDogTest::name()
{
	return "ThreadPoolWatchDogTest";
}

void ThreadPoolWatchDogTest::receiveMessage(const BlackBox::IMessage& message)
{
	if (message.type() == BlackBox::IMessage::ThreadKilled)
	{
		std::lock_guard<std::mutex> locker(m_mutex);

		const BlackBox::ThreadKilledMessage& actualMessage = 
			static_cast<const BlackBox::ThreadKilledMessage&>(message);

		std::cout << "Thread #" << actualMessage.id() << " killed" << std::endl;

		m_restartedThreads[actualMessage.id()] = true;
	}

	m_threadKilledCondition.notify_one();
}

void ThreadPoolWatchDogTest::testFunction()
{
	using namespace std::chrono_literals;

	std::this_thread::sleep_for(10s);
}

const std::size_t ThreadPoolWatchDogTest::s_tasksCounter = std::thread::hardware_concurrency();

const std::size_t ThreadPoolWatchDogTest::s_functionTimeoutMs = 5000;

}