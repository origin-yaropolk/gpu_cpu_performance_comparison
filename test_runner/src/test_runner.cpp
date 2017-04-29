#include "test_runner.h"
#include <iostream>
#include <string>

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
		std::cout << "****************************************************" << std::endl;
		std::cout << testCase->name() << " started:" << std::endl << std::endl;

		testCase->run();

		std::cout << "****************************************************" << std::endl << std::endl;
	}
}

}