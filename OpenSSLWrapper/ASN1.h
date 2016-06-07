#pragma once
#include "Util.h"
#include "BigNumber.h"
class Integer : public ASN1_INTEGER
{
	ASN1_INTEGER * _Integer;
	ASN1_INTEGER * create();
public:
	Integer();
	Integer(long);
	Integer(ASN1_INTEGER*);
	Integer(BigNumber&);
	long ToLong();
	void Write(const char* buffer);
};

class Time : public ASN1_TIME
{
	ASN1_INTEGER * _Integer;
	ASN1_INTEGER * create();
public:
	Time();
	Time(long);
	Time(ASN1_TIME*);
	Time(BigNumber&);
	long ToLong();
	void Write(const char* buffer);
};




