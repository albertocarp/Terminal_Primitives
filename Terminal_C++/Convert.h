#pragma once
#include "defs.h"
class Convert
{
public:
	static const short CHUNK_DATA = 20;
public:
	static char* decToBin(float input, const short & out_size);
};

