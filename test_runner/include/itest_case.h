#pragma once

#include <string>

namespace TestCase
{

class ITestCase
{
public:
	virtual bool run() = 0;

	virtual std::string name() = 0;
};

}
