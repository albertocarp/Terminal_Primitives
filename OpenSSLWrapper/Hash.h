#pragma once


#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include"link.h"

class Hash//utils
{
public:
	Hash();
	static uch*	 sha1_v1(	uch*  string,		uint strlen,	 uint hex_format = 0);
	static uch*  sha1_v2(	uch*  string,		uint strlen,	 uint hex_format = 0);
	
	static uch*  sha256_v1(	uch*  string,		uint strlen,	 uint hex_format = 0);
	static uch*  sha256_v2(	uch*  string,		uint strlen,	 uint hex_format = 0);

	static uch*  sha512_v1(	uch*  string,		uint strlen,	 uint hex_format = 0);
	static uch*  sha512_v2(	uch*  string,		uint strlen,	 uint hex_format = 0);

	static uch*  hmac_v1(	uch*key, uint keyLen, uch*data, uint dataLen, uint hex_format = 0);
	static uch*  hmac_v2(	uch*key, uint keyLen, uch*data, uint dataLen, uint hex_format = 0);

	~Hash();
};

