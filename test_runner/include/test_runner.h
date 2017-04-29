#pragma once

#include "test_cases.h"
#include <vector>

namespace TestRunnerEnv
{

class TestRunner
{
public:
	TestRunner();

	void start();

private:
	std::vector<std::unique_ptr<TestCase::ITestCase>> m_testCaseItems;
};

}
