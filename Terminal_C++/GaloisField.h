#pragma once
#include "defs.h"
class GaloisField
{
	short p[21];
	short n, k,m,length,t,d;
	short alpha_to[512];
	short index_of[512];
	int rdncy;
private:
	void precomputeInput();
public:
	void getGaloisField();
	short* generatePolinom();
	int getLastRedundancy();
	GaloisField(short M,short length,short t);
};


