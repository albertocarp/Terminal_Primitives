#pragma once
#include "defs.h"
class TestCaseException :
	public std::exception
{
public:
	TestCaseException();
	TestCaseException(const std::string&);
	TestCaseException(const std::string&, long msg);
	~TestCaseException();
};


