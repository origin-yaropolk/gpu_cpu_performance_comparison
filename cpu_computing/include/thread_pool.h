#pragma once

#include <condition_variable>
#include <future>
#include <thread>
#include <mutex>
#include <queue>
#include <map>
#include <iostream>
#include <cstddef>
#include <cassert>
#include <windows.h>

namespace BlackBox
{

#ifdef TEST_RUNNER_ENVIRONMENT

class IMessage
{
public:
	enum
	{
		ThreadKilled
	};

	virtual int type() const noexcept = 0;
};

class ThreadKilledMessage : public IMessage
{
public:
	ThreadKilledMessage(std::size_t id)
		: m_threadId(id)
	{
	}

	std::size_t id() const noexcept
	{
		return m_threadId;
	}
	
	virtual int type() const noexcept override
	{
		return ThreadKilled;
	}

private:
	std::size_t m_threadId;
};

class TestReceiver
{
public:
	virtual void receiveMessage(const IMessage& message) = 0;
};


class TestMessager
{
public:
	void SendMessage(const IMessage& message) noexcept
	{
		for (TestReceiver* receiver : m_receivers)
		{
			receiver->receiveMessage(message);
		}
	}

	void addReceiver(TestReceiver* receiver)
	{
		if (std::find(std::begin(m_receivers), std::end(m_receivers), receiver) == std::end(m_receivers))
		{
			m_receivers.push_back(receiver);
		}
	}

private:
	std::vector<TestReceiver*> m_receivers;
};

#endif

template <typename ReturnT>
class ThreadPool

{
	/** DATA AND INTERNAL DATA TYPES **/
private:

	// implemented only move semantic
	struct TaskWrapper
	{
		std::packaged_task<ReturnT()> task;
		bool completeSignal;
	};

	// implemented only move semantic
	struct ThreadSynchronization
	{
		std::condition_variable condition;
		std::mutex mutable mutex;
		std::queue<TaskWrapper> queue;
		std::atomic_bool threadInactive = false;
	};

protected:

	struct WatchDog
	{
		std::mutex watchDogMutex;
		std::condition_variable watchDogCondition;
		std::thread watchDogThread;

		std::chrono::time_point<std::chrono::system_clock> timePoint;
		std::atomic_bool taskStarted;
		std::atomic_bool completeWatchDogThread;
	};

	size_t m_threadsNumber;
	std::vector<std::thread> m_threads;
	std::vector<WatchDog> m_watchDogs;
	std::size_t m_functionTimeoutMs;

private:
	std::map<size_t, ThreadSynchronization> m_threadsSynchronize;

protected:

	ThreadPool()
		: ThreadPool(std::thread::hardware_concurrency() - 1)
	{
	}
	
	ThreadPool(size_t threadsNumber, size_t functionTimeoutMs = 60 * 5 * 1000)
		: m_threadsNumber(threadsNumber)
		, m_threads(threadsNumber)
		, m_watchDogs(threadsNumber)
		, m_functionTimeoutMs(functionTimeoutMs)
	{
		m_threadsNumber = threadsNumber;

		for (size_t i = 0; i < m_threadsNumber; ++i)
		{
			m_threadsSynchronize[i];
			m_threads[i] = std::thread(&ThreadPool::taskHandler, this, i);

			m_watchDogs[i].watchDogThread = std::thread(&ThreadPool::watchDog, this, i);
			m_watchDogs[i].completeWatchDogThread.store(false);
		}
	}

public:

	typedef std::shared_future<ReturnT> ResultType;

	//
	// If this function would be called in test runner environment
	// Then will be crash when program closes
	//
	static ThreadPool& instance()
	{
		static ThreadPool threadPool;
		return threadPool;
	}

	~ThreadPool()
	{

#ifndef TEST_RUNNER_ENVIRONMENT
		completeAllThreads();
#endif

	}

	template <typename F, typename... Args>
	ResultType addTask(F f, Args&&... params)
	{
		TaskWrapper newTask = binder(f, std::forward<Args>(params)...);
		ResultType futureObject = newTask.task.get_future();
		ThreadSynchronization& ts = findMinimumLoadedThread();

		putTask(ts, std::move(newTask));
		ts.condition.notify_one();

		return futureObject;
	}

	size_t workingThreads() const
	{
		return m_threadsNumber;
	}

	void waitAll()
	{
		while (!isAllQueuesEmpty())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

private:

	void watchDog(size_t id)
	{
		using namespace std::chrono_literals;
		
		while(!m_watchDogs[id].completeWatchDogThread.load())
		{
			while (m_watchDogs[id].taskStarted)
			{
				std::unique_lock<std::mutex> locker(m_watchDogs[id].watchDogMutex);

				if (m_watchDogs[id].watchDogCondition.wait_until(locker, m_watchDogs[id].timePoint + std::chrono::milliseconds(m_functionTimeoutMs)) ==
					std::cv_status::timeout)
				{
					std::cout << "Thread #" << id << " restarted!" << std::endl;

					restartThread(id);
				}
			}

			std::this_thread::sleep_for(1ms);
		}
	}

	// this method potentially dangerous
	void restartThread(size_t id)
	{
		m_threadsSynchronize[id].threadInactive.store(true);
		m_watchDogs[id].taskStarted.store(false);

		TerminateThread(m_threads[id].native_handle(), 0);
		m_threads[id].join();

		m_threadsSynchronize[id].queue.pop();

		m_threads[id] = std::thread(&ThreadPool::taskHandler, this, id);

		m_threadsSynchronize[id].threadInactive.store(false);
	}

	/** executes passed tasks */
	void taskHandler(size_t id)
	{
		for (;;)
		{
			ThreadSynchronization& ts = m_threadsSynchronize[id];
			std::unique_lock<std::mutex> locker{ ts.mutex };
			
			ts.condition.wait(locker, [&ts] { return !ts.queue.empty(); });
			auto callable = getTask(ts, std::defer_lock);
			
			locker.unlock();

			// first check
			// if thread was slept and received the complete signal
			if (callable.second.completeSignal)
			{
				break;
			}

			// for correct exit from loop
			bool double_check_complete = false;

			// executes all tasks from queue
			for (;;)
			{
				// double check
				// if this thread works and gets the complete signal
				// complete signal will be sent without valid packaged_task
				// and if that task to attempt to execute
				// the future_error exception will be thrown
				if (callable.second.completeSignal)
				{
					double_check_complete = true;
					break;
				}

				m_watchDogs[id].timePoint = std::chrono::system_clock::now();
				m_watchDogs[id].taskStarted.store(true);

				callable.second.task();

				m_watchDogs[id].taskStarted.store(false);
				m_watchDogs[id].watchDogCondition.notify_one();
				
				//
				// Remove task from queue must be after
				// it executed !!!
				// Otherwise possibly would be crash
 				//
				if (!m_threadsSynchronize[id].threadInactive.load())
				{
					ts.queue.pop();
				}

				callable = getTask(ts);

				if (callable.first == false)
				{
					break;
				}
			}

			// for correct exit from loop
			if (double_check_complete)
			{
				break;
			}
		}
	}

protected:


#ifdef TEST_RUNNER_ENVIRONMENT
	public:
#endif
	// send complete signal for specified thread
	void completeThread(size_t id)
	{
		ThreadSynchronization& ts = m_threadsSynchronize[id];

		TaskWrapper cs{ {}, true };
		putTask(ts, std::move(cs));

		ts.condition.notify_one();
	}

	void completeAllThreads()
	{
		for (size_t i = 0; i < m_threadsNumber; ++i)
		{
			completeThread(i);

			if (m_threads[i].joinable())
			{
				m_threads[i].join();
			}

			m_watchDogs[i].completeWatchDogThread.store(true);

			if (m_watchDogs[i].watchDogThread.joinable())
			{
				m_watchDogs[i].watchDogThread.join();
			}
		}
	}

private:

	/** ATOMIC OPERATIONS **/

	// pops the task with blocking
	std::pair<bool, TaskWrapper> getTask(ThreadSynchronization& ts)
	{
		std::lock_guard<std::mutex> locker{ ts.mutex };
		bool successFlag = !ts.queue.empty();
		TaskWrapper tw{ {}, false };

		if (successFlag)
		{
			tw = std::move(ts.queue.front());
		}

		return std::make_pair(successFlag, std::move(tw));
	}

	// pops the task without blocking
	std::pair<bool, TaskWrapper> getTask(ThreadSynchronization& ts, std::defer_lock_t strategy)
	{
		(void)strategy;

		bool successFlag = !ts.queue.empty();
		TaskWrapper tw{ {}, false };

		if (successFlag)
		{
			tw = std::move(ts.queue.front());
		}

		return std::make_pair(successFlag, std::move(tw));
	}

	void putTask(ThreadSynchronization& ts, TaskWrapper tw)
	{
		std::lock_guard<std::mutex> locker{ ts.mutex };
		ts.queue.push(std::move(tw));
	}

	void putTask(ThreadSynchronization& ts, TaskWrapper tw, std::defer_lock_t strategy)
	{
		(void)strategy;
		ts.queue.push(std::move(tw));
	}

	/** HELPER FUNCTIONS **/

	template <typename F, typename... Args>
	TaskWrapper binder(F f, Args&&... params)
	{
		auto newCallableObject = std::bind(f, std::forward<Args>(params)...);
		TaskWrapper newWrapper{ std::packaged_task<ReturnT()>{newCallableObject}, false };
		
		return newWrapper;
	}

	ThreadSynchronization& findMinimumLoadedThread()
	{
		m_threadsSynchronize[0].mutex.lock();
		auto smallest = m_threadsSynchronize[0].queue.size();
		m_threadsSynchronize[0].mutex.unlock();

		size_t minElem = 0;

		for (size_t i = 1; i < m_threadsNumber; ++i)
		{
			std::lock_guard<std::mutex> locker{ m_threadsSynchronize[i].mutex };
			if (m_threadsSynchronize[i].queue.size() < smallest &&
				!m_threadsSynchronize[i].threadInactive.load())
			{
				smallest = m_threadsSynchronize[i].queue.size();
				minElem = i;
			}
		}

		return m_threadsSynchronize[minElem];
	}

	bool isAllQueuesEmpty()
	{
		for (size_t i = 0; i < m_threadsNumber; ++i)
		{
			std::lock_guard<std::mutex> locker{ m_threadsSynchronize[i].mutex };
			if (m_threadsSynchronize[i].queue.size())
			{
				return false;
			}
		}

		return true;
	}
};

template <typename ReturnT>
class TestingThreadPool : public ThreadPool<ReturnT>
{
	using Base = ThreadPool<ReturnT>;

public:

	TestingThreadPool()
		: Base()
	{
	}

	TestingThreadPool(std::size_t threadsNumber, std::size_t functionTimeoutMs = 60 * 5 * 1000)
		: Base(threadsNumber, functionTimeoutMs)
	{
	}

	//
	// here I cannot to write: using Base::Base;
	// because I'm using Visual Studio. Hello from 2017 ;)
	//

	using Base::WatchDog;

	using Base::completeThread;
	using Base::completeAllThreads;

	using Base::m_threadsNumber;
	using Base::m_threads;
	using Base::m_watchDogs;
	using Base::m_functionTimeoutMs;
};

} // end of namespace BlackBox