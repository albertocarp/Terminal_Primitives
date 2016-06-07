#pragma once
#include "Apdu.h"
#include "SmartCardException.h"
class ApduResponse :
	public Apdu
{
	StatusSW *status;

public:
	ApduResponse(byte * data, byte datalen);
	ApduResponse(byte * data, byte datalen,double time_elapsed);
	~ApduResponse();
	virtual byte getExpectedLen()const;
	std::string toString()const;
	virtual bool hasStatus();
	virtual StatusSW getStatus();
	virtual void wrap(byte* pData, word pLength);
	virtual void wrap(vArray* data);

};

