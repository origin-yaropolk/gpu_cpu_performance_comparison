#pragma once

#include "itest_case.h"
#include "pimpl_helper.h"

namespace BlackBox
{

template <typename ReturnT>
class TestingThreadPool;

}

namespace TestCase
{

class ThreadPoolDestructionTest : public ITestCase
{
public:
	ThreadPoolDestructionTest();

	virtual bool run() override;
	virtual std::string name() override;

private:
	using thread_pool_type = BlackBox::TestingThreadPool<void>;

	std::unique_ptr<PimplHelper<thread_pool_type>> m_testingObject;
};

}