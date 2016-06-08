#pragma once
#include "TestCase.h"
#include "IssuerParameters.h"

template <typename instance>
class SingleAutomaticTest
{
	instance object;
public:
	void Test();
	SingleAutomaticTest();
	~SingleAutomaticTest();
};


template<typename instance>
void SingleAutomaticTest<instance>::Test()
{
	try
	{
		bool ret  = object.Test();
		if (ret == true)
			std::cout << "Test passed";
		else
			std::cout << "Test failed";
	}
	catch (std::exception exc)
	{
		std::cout << exc.what();
	}

}
template<typename instance>
SingleAutomaticTest<instance>::SingleAutomaticTest()
{

}
template<typename instance>
SingleAutomaticTest<instance>::~SingleAutomaticTest()
{
}