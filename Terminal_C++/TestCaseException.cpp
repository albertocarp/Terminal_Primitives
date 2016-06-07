#include "TestCaseException.h"



TestCaseException::TestCaseException()
{
}

TestCaseException::TestCaseException(const std::string & msg)
	:std::exception(msg.c_str())
{
}

TestCaseException::TestCaseException(const std::string & msg, long code)
	: std::exception(msg.c_str(), code)
{
}


TestCaseException::~TestCaseException()
{
}