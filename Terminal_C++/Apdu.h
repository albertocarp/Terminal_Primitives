#pragma once
#include "defs.h"
#include "StatusSW.h"
#include "CardSession.h"
class Apdu
{
public:
	static final ISO7816_OFFSET_CLA = 0;
	static final ISO7816_OFFSET_INS = 1;
	static final ISO7816_OFFSET_P1 = 2;
	static final ISO7816_OFFSET_P2 = 3;
	static final ISO7816_OFFSET_LC = 4;
	static final ISO7816_OFFSET_CDATA = 5;
protected:
	byte * rawData;
	word dataLength; 
	double time_elpsed;
public:
	const	byte*	 getData()const;
	byte*			 getNonConstantData();
	virtual const	word	 getDataLength()const;
	virtual const	word	 getApduLength()const;
	virtual byte	 getExpectedLen()const = 0;
	virtual bool	 hasStatus()=0;
	virtual StatusSW getStatus()=0;
	virtual void wrap(byte* pData, word pLength)=0;
	virtual void wrap(vArray* data)=0;
	virtual void update(Apdu* data);
	bool hasData();
	virtual std::string toString()const=0;
	double getTimeElapsed();
	Apdu();
	~Apdu();
};

