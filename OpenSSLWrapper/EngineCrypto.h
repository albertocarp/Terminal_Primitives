#pragma once
#include"Util.h"
class EngineCrypto
{
public:
	//Returns output encrypted string 
	// Set outoutLen output encData
	unsigned char* AES_ECB_Encrypt(unsigned char*InputString,unsigned int inpLen ,unsigned int& outputLen,unsigned char* key,unsigned int keylen); 
	unsigned char* AES_ECB_Decrypt(unsigned char*InputString,unsigned int inpLen ,unsigned int& outputLen,unsigned char* key,unsigned int keylen);

	void AES_ECB_Encrypt(char*InFile,char* OutFile,unsigned char* key,unsigned int keyLen);
	void AES_ECB_Decrypt(char*InFile,char* OutFile,unsigned char* key,unsigned int keyLen);

	unsigned char* AES_CBC_Encrypt(unsigned char*InputString,unsigned int inpLen ,unsigned int& outputLen,unsigned char* key,unsigned int keylen,unsigned char* IV); 
	unsigned char* AES_CBC_Decrypt(unsigned char*InputString,unsigned int inpLen ,unsigned int& outputLen,unsigned char* key,unsigned int keylen,unsigned char* IV); 
	
	unsigned char* AES_CTR_Encrypt(unsigned char*InputString,unsigned int inpLen ,unsigned int& outputLen,unsigned char* key,unsigned char* IV); 
	unsigned char* AES_CTR_Decrypt(unsigned char*InputString,unsigned int inpLen ,unsigned int& outputLen,unsigned char* key,unsigned char* IV); 
	
	unsigned char* DES_3ECB_Encrypt(unsigned char*InputString,unsigned int inpLen ,unsigned int& outputLen,const unsigned char** keys); 
	unsigned char* DES_3ECB_Decrypt(unsigned char*InputString,unsigned int inpLen ,unsigned int& outputLen,const unsigned char** keys);

	unsigned char* DES_cfb64_Encript(unsigned char *Key, unsigned char *InputString, unsigned int size);
    unsigned char* DES_cfb64_Decrypt(unsigned char *Key, unsigned char *InputString, unsigned int size);

		unsigned char*DES_3CBC_Encrypt(unsigned char*InputString, unsigned int inpLen, unsigned int& outputLen, const unsigned char** keys, const char* iv);
	unsigned char*DES_3CBC_Decrypt(unsigned char*InputString, unsigned int inpLen, unsigned int& outputLen, const unsigned char** keys, const char* iv);

	unsigned char*DES_cbc_Encrypt(unsigned char*InputString, unsigned int inpLen, unsigned int& outputLen, unsigned char* keys, unsigned char *iv);
	unsigned char*DES_cbc_Decrypt(unsigned char*InputString, unsigned int inpLen, unsigned int& outputLen, unsigned char* keys, unsigned char*iv);
public:
	EngineCrypto(void);
	~EngineCrypto(void);
private:

};

