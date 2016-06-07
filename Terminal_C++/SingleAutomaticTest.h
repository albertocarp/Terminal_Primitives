#pragma once
#include "TestCase.h"
#include "IssuerParameters.h"
template <typename instance>
class SingleAutomaticTest
{
	instance object;
public:
	bool TestFunction(const std::string& name);
	SingleAutomaticTest();
	~SingleAutomaticTest();
};


template<typename instance>
bool SingleAutomaticTest<instance>::TestFunction(const std::string& name)
{
	std::string noCh = name.substr(1, name.size() - 1);
	//Test(, byte*, nullptr, nullptr, 1);
}
template<typename instance>
SingleAutomaticTest<instance>::SingleAutomaticTest()
{
}
template<typename instance>
SingleAutomaticTest<instance>::~SingleAutomaticTest()
{
}