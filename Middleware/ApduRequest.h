#pragma once
#include "Apdu.h"
#include "SmartCardException.h"
class ApduRequest :
	public Apdu
{
	byte Cla;
	byte Ins;
	byte P1;
	byte P2;
	byte Lc;
	byte Le;
public:
	ApduRequest(byte Cla, byte Ins, byte P1, byte P2, byte Lc,byte Le);
	ApduRequest(byte Cla, byte Ins, byte P1, byte P2, byte Lc);
	ApduRequest(byte Cla, byte Ins, byte P1, byte Lc);
	virtual byte getExpectedLen()const;
	virtual bool hasStatus();
	virtual StatusSW getStatus();
	virtual void wrap(byte* pData, word pLength);
	virtual void wrap(vArray* data);
	virtual void update(Apdu* data);
	virtual const	byte	 getDataLength()const override;
	virtual std::string toString()const;
	~ApduRequest();
};

