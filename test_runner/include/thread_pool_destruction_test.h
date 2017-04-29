#pragma once

#include "itest_case.h"

namespace BlackBox
{

template <typename ReturnT>
class ThreadPool;

}

namespace TestCase
{

class ThreadPoolDestructionTest : public ITestCase
{
public:
	ThreadPoolDestructionTest();

	virtual void run() override;
	virtual std::string name() override;

private:
	using thread_pool_type = BlackBox::ThreadPool<void>;

	thread_pool_type* m_testingObject;
};

}