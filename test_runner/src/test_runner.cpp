#define TEST_RUNNER_ENVIRONMENT

#include "test_runner.h"
#include "thread_pool.h"
#include <iostream>
#include <iomanip>
#include <string>

namespace TestRunnerEnv
{

TestRunner::TestRunner()
{
<<<<<<< HEAD
	m_testCaseItems.push_back(std::make_unique<TestCase::CorrectnessCpuCalculationsTest>());
	//m_testCaseItems.push_back(std::make_unique<TestCase::ThreadPoolDestructionTest>());
	//m_testCaseItems.push_back(std::make_unique<TestCase::ThreadPoolExecutionTest>());
	//m_testCaseItems.push_back(std::make_unique<TestCase::ThreadPoolWatchDogTest>());
=======
	//m_testCaseItems.push_back(std::make_unique<TestCase::CorrectnessCpuCalculationsTest>());
	m_testCaseItems.push_back(std::make_unique<TestCase::ThreadPoolDestructionTest>());
	m_testCaseItems.push_back(std::make_unique<TestCase::ThreadPoolExecutionTest>());
	m_testCaseItems.push_back(std::make_unique<TestCase::ThreadPoolWatchDogTest>());
	m_testCaseItems.push_back(std::make_unique<TestCase::CorrectnessGpuCalculationsTest>());
>>>>>>> d1840fc21a7068cd20a55273c19b205c9f6b9bef
}

void TestRunner::start()
{
	std::string border;
	border.assign(70, '_');

	for (auto& testCase : m_testCaseItems)
	{
		std::cout << border << std::endl;

		std::cout << std::setw(70) << std::left << std::setfill('.') << testCase->name() + " started:" << std::endl << std::endl;

		if (testCase->run())
		{
			std::cout << std::setw(70) << std::left << std::setfill('.') << "Test successful passed!" << std::endl;
		}
		else
		{
			std::cout << std::setw(70) << "Test failed!" << std::setfill('.') << std::endl;
		}

		testCase.reset();

		std::cout << border << std::endl << std::endl << std::endl;
	}
}

}