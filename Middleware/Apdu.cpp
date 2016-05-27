#include "Apdu.h"



const byte * Apdu::getData()const 
{
	return rawData;
}

byte * Apdu::getNonConstantData()
{
	return rawData;
}

const byte Apdu::getDataLength()const
{
	return dataLength;
}

const byte Apdu::getApduLength() const
{
	return dataLength;
}

void Apdu::update(Apdu * data)
{
}

bool Apdu::hasData()
{
	if (dataLength > 0)
		return true;
	return false;
}

double Apdu::getTimeElapsed()
{
	return time_elpsed;
}

Apdu::Apdu()
{
}


Apdu::~Apdu()
{
}
