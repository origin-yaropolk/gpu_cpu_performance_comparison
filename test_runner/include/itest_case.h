#pragma once

#include <string>

namespace TestCase
{

class ITestCase
{
public:
	virtual void run() = 0;

	virtual std::string name() = 0;
};

}
