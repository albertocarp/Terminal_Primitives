#pragma once
#include "defs.h"
class SmartCardException :
	public std::exception
{
public:
	SmartCardException();
	SmartCardException(const std::string&);
	SmartCardException(const std::string&,long msg);
	~SmartCardException();
};

