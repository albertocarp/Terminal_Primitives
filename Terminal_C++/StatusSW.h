#pragma once
#include "defs.h"
#include "SmartCardException.h"
class StatusSW
{
private:
	byte sw1;
	byte sw2;
public:
	const std::string toString();
	StatusSW(byte sw1,byte sw2);
	byte SW1();
	byte SW2();
	void validate();
	bool operator==(const StatusSW & rhs);
	~StatusSW();
};

