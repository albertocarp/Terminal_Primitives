#pragma once
#include "link.h"
class Util
{
public:
	static void _print_hexa_buffer(unsigned char *buffer, unsigned int len);
	static int _read_from_file (char *filename, unsigned char **data, unsigned int *len);
	static int _write_to_file (char *filename, unsigned char *data, unsigned int len);
	static unsigned char* generateRandom(int bytes);
	static unsigned char* xor_enc(unsigned char* in,unsigned char* in2,unsigned int len);
	uint32_t static RCS32(uint32_t w,unsigned int times );
    uint32_t static LCS32(uint32_t w,unsigned int times );
	uint64_t static RCS64(uint64_t w,unsigned int times );
    uint64_t static LCS64(uint64_t w,unsigned int times );
	static void ToCharArray(uint32_t* _long,unsigned char* buf);
	static void ToCharArray(uint64_t* _long,unsigned char* buf);
	static void ToUInt32(unsigned char* data,uint32_t& ret);
	static void ToUInt64(unsigned char* data,uint64_t&ret);
	static string& ToString(int);
	Util(void);
	~Util(void);
};

