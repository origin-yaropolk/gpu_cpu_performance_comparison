#pragma once

#include <mutex>

namespace BlackBox
{

template <typename ReturnT>
class ThreadPool;

}

namespace TestCases
{

class ThreadPoolDestructionTest
{
public:
	ThreadPoolDestructionTest();

	void run();

private:
	static void testFunction();
	
private:
	static std::size_t m_executionCounter;
	static std::mutex m_mutex;

private:
	using thread_pool_type = BlackBox::ThreadPool<void>;

	thread_pool_type* m_testingObject;
};

}