#include "SmartCardException.h"



SmartCardException::SmartCardException()
	: exception()
{
}

SmartCardException::SmartCardException(const std::string & rhs)
	:std::exception(rhs.c_str())
{
}

SmartCardException::SmartCardException(const std::string & rhs, long code)
	:std::exception(rhs.c_str(),code)
{
}


SmartCardException::~SmartCardException()
{
}
