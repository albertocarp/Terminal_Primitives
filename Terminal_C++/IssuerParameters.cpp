#include "IssuerParameters.h"

using namespace Auth;

byte * Auth::IssuerParameters::getG()
{
	return FieldOrderPrime.G;
}

byte * Auth::IssuerParameters::getQ()
{
	return FieldOrderPrime.Q;
}

byte * Auth::IssuerParameters::getP()
{
	return FieldOrderPrime.P;
}

byte * Auth::IssuerParameters::getUidP()
{
	return UidP;
}

byte * Auth::IssuerParameters::getUidH()
{
	return UidH;
}

byte * Auth::IssuerParameters::getSpecification()
{
	return Specification;
}

void Auth::IssuerParameters::setP(byte * P, size_t P_Size)
{
	FieldOrderPrime.P = new byte[P_Size];
	memcpy(FieldOrderPrime.P, P, P_Size);
	FieldOrderPrime.P_Size = P_Size;
}

void Auth::IssuerParameters::setQ(byte * Q, size_t Q_Size)
{
	FieldOrderPrime.Q = new byte[Q_Size];
	memcpy(FieldOrderPrime.Q, Q, Q_Size);
	FieldOrderPrime.Q_Size = Q_Size;
}

void Auth::IssuerParameters::setG(byte * G, size_t G_Size)
{
	FieldOrderPrime.G = new byte[G_Size];
	memcpy(FieldOrderPrime.G, G, G_Size);
	FieldOrderPrime.G_Size = G_Size;
}

void Auth::IssuerParameters::setUidP(byte * UidP, size_t UidP_Size)
{
	this->UidP = new byte[UidP_Size];
	memcpy(this->UidP, UidP, UidP_Size);
	this->UidP_Size = UidP_Size;
}

void Auth::IssuerParameters::setUidH(byte * UidH, size_t UidH_Size)
{
	this->UidH = new byte[UidH_Size];
	memcpy(this->UidH, UidH, UidH_Size);
	this->UidH_Size = UidH_Size;
}

void Auth::IssuerParameters::setSpecification(byte * Specification, size_t Specification_Size)
{
	this->Specification = new byte[Specification_Size];
	memcpy(this->Specification, Specification, Specification_Size);
	this->Specification_Size = Specification_Size;
}

size_t Auth::IssuerParameters::getPSize()
{
	return FieldOrderPrime.P_Size;
}

size_t Auth::IssuerParameters::getQSize()
{
	return FieldOrderPrime.Q_Size;
}

size_t Auth::IssuerParameters::getGSize()
{
	return FieldOrderPrime.G_Size;
}

IssuerParameters::IssuerParameters()
{
}


IssuerParameters::~IssuerParameters()
{
}
