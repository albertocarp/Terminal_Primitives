#pragma once
#include "Apdu.h"
class ApduSelection :
	public Apdu
{
	StatusSW *status;
public:
	ApduSelection(byte* selectAID,word selectLength);
	~ApduSelection();
	static final* DEFAULT_AID;
	virtual bool hasStatus();
	virtual StatusSW getStatus();
	byte getExpectedLen() const;
	virtual void wrap(byte* pData, word pLength);
	virtual void wrap(vArray* data);
	virtual std::string toString()const;
};

