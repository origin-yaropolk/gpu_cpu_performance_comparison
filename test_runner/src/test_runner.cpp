#include "test_runner.h"

namespace TestRunnerEnv
{

TestRunner::TestRunner()
{
	m_testCaseItems.push_back(std::make_unique<TestCase::ThreadPoolDestructionTest>());
	m_testCaseItems.push_back(std::make_unique<TestCase::ThreadPoolExecutionTest>());
}

void TestRunner::start()
{
	for (auto const& testCase : m_testCaseItems)
	{
		testCase->run();
	}
}

}