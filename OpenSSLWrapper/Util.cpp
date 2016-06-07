#include "Util.h"


Util::Util(void)
{
}
Util::~Util(void)
{
}


void Util::_print_hexa_buffer(unsigned char *buffer, unsigned int len)
{
	int i;
	fprintf (stdout, "\n");	
	for (i = 0; i < (int)len; i++)
		fprintf (stdout, "%02X ", buffer[i]);
	fprintf (stdout, "\n");
}
int Util::_write_to_file (char *filename, unsigned char *data, unsigned int len)
{
	if (data == NULL)
		return STATUS_ERR_IO;

	FILE *fp = fopen (filename, "wb");	
	if (fp == NULL)
		return STATUS_ERR_IO;

	fwrite (data, 1, len, fp);

	fclose (fp);

	return STATUS_OK;
}
int Util::_read_from_file (char *filename, unsigned char **data, unsigned int *len)
{
	if (data == NULL || len == NULL)
		return STATUS_ERR_IO; 
	FILE *fp = fopen (filename, "rb");
	if (fp == NULL)
		return STATUS_ERR_IO;

	fseek (fp, 0, SEEK_END);
	*len = (unsigned int) ftell (fp);
	fseek (fp, 0, SEEK_SET);

	*data = (unsigned char *) malloc (*len);
	
	fread (*data, 1, *len, fp);
	fclose (fp);

	return STATUS_OK;
}
unsigned char* Util::generateRandom(int bytes)
{
	unsigned char* out = (unsigned char*)malloc(bytes);
	for(int i = 0;i<bytes;i++)
	{
		out[i] = rand()%255;
	}
	return out;
}
unsigned char* Util::xor_enc(unsigned char* in,unsigned char* in2,unsigned int len)
{
	unsigned char* x_enc = (unsigned char*)malloc(len);
	for(size_t i=0;i<len;i++)
		x_enc[i] = in[i]^in2[i];
	return x_enc;
}

void Util::ToCharArray(uint32_t* _long,unsigned char* buf)
{
	   buf[0] = *_long >> 24;
	   buf[1] = *_long >> 16;
	   buf[2] = *_long >> 8;
	   buf[3] = *_long;
}
void Util::ToUInt32(unsigned char* data,uint32_t& ret)
{
  ret = ((data[0] << 24) | (data[1] <<16) | ( data[2] << 8) | data[3]);
}

void Util::ToCharArray(uint64_t* _long,unsigned char* buf)
{
	   buf[0] = (unsigned char)(*_long >> 56);
	   buf[1] = (unsigned char)(*_long >> 48);
	   buf[2] = (unsigned char)(*_long >> 40);
	   buf[3] = (unsigned char)(*_long >>32);
	   buf[4] = (unsigned char)(*_long >> 24);
	   buf[5] = (unsigned char)(*_long >> 16);
	   buf[6] = (unsigned char)(*_long >> 8);
	   buf[7] = (unsigned char)(*_long);
}
void Util::ToUInt64(unsigned char* data,uint64_t& ret)
{
  ret = (uint64_t)((data[0] << 56) | (data[1] <<48) | ( data[2] << 40) | data[3] << 32 | data[4] << 24 | data[5]<<16 | data[6] << 8 | data[7]);
}

uint32_t Util::RCS32(uint32_t w,unsigned int times )
{
 return (w>>times | w<<(32-times));
}
uint32_t Util::LCS32(uint32_t w,unsigned int times )
{
  return (w<<times | w>>(32-times));
}

uint64_t Util::RCS64(uint64_t w,unsigned int times )
{
 return (w>>times | w<<(64-times));
}
uint64_t Util::LCS64(uint64_t w,unsigned int times )
{
  return (w<<times | w>>(64-times));
}
string& Util::ToString(int val)
{
	char buffer[200];
	_itoa(val,buffer,10);
	return *new string(buffer);
}