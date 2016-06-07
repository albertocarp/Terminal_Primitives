#include "Helper.h"
#include "defs.h"

void Helper:: xor (char * x1, char * x2,int len)
{
	for (short i = 0; i < len; i++)
		x1[i] = x1[i] ^ x2[i];
}

void Helper::print_hexa(char * buffer, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		printf("%02X ", (buffer[i] & 0x0000000ff));
	}
	printf("\n");
}
