#include "StatusSW.h"



const std::string StatusSW::toString()
{
	std::stringstream ss;
	ss << "SW1 : ";
	ss << "0x" << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(this->sw1);
	ss << " SW2 : ";
	ss << "0x" << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(this->sw2);
	return ss.str();
}

StatusSW::StatusSW(byte sw1,byte sw2)
{
	this->sw1 = sw1;
	this->sw2 = sw2;
}

byte StatusSW::SW1()
{
	return sw1;
}

byte StatusSW::SW2()
{
	return sw2;
}

void StatusSW::validate()
{
	if (this->sw1 != 0x90 && this->sw2 != 0x00)
	{
		throw new SmartCardException("Invalid status card");
	}
}

bool StatusSW::operator==(const StatusSW & rhs)
{
	if (this->sw1 == rhs.sw1 && this->sw2 == rhs.sw2)
		return true;
	return false;
}


StatusSW::~StatusSW()
{
}
