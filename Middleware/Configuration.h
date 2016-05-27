#pragma once
#include "defs.h"
class Configuration
{
public:
	static void init();
	static byte* selectAID;
	static const int selectAidLen=14;
	static char initialized;

	static final APPLICATION_CLA = 0x00;
	static final CIPHER_LIGTWEIGHT = 0x11;
	static final UPROVE_IDENTIFIER = 0x22;
	static final HASH_FUNCTIONS = 0x24;

};

