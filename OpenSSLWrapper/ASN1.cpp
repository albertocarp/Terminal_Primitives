#include "ASN1.h"


Integer::Integer()
{
	
}

ASN1_INTEGER* Integer::create()
{
	 _Integer =  ASN1_INTEGER_new();
	 return  _Integer;
}

Integer::Integer(long value)
{
	create();
	ASN1_INTEGER_set(this->_Integer, value);
}

long Integer::ToLong()
{
	return ASN1_INTEGER_get(this);
}

Integer::Integer(ASN1_INTEGER* integer)
{
	this->_Integer = integer;
}