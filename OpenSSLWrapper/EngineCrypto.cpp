#include "EngineCrypto.h"


EngineCrypto::EngineCrypto(void)
{
}
EngineCrypto::~EngineCrypto(void)
{
}

unsigned char* EngineCrypto::AES_ECB_Encrypt(unsigned char*InputString,unsigned int inpLen,unsigned int& outputLen,unsigned char* key,unsigned int keylen) //Encrypt string 
{
	int status = STATUS_OK;
	unsigned char *clearData = InputString;
	unsigned int clearLen = inpLen;
	unsigned char *encData = NULL;
	unsigned int encLen;
	unsigned char *pClearData = clearData;
    /*** criptez AES datele */
	AES_KEY		aesKey;
	status = (AES_set_encrypt_key(key, keylen * 8, &aesKey) != 0);
	if (status != STATUS_OK)
		 throw new std::exception("Eroare setare cheie AES");
	unsigned char inblk[_AES_BLOCK_LEN], outblk[_AES_BLOCK_LEN];
	encData = (unsigned char *) malloc (0);
	encLen = 0;
	while (clearLen >= _AES_BLOCK_LEN)
	{
		memcpy (inblk, pClearData, _AES_BLOCK_LEN);	
		
		AES_encrypt (inblk, outblk, &aesKey);
			
		encData = (unsigned char *) realloc (encData, encLen + _AES_BLOCK_LEN);
		memcpy (encData + encLen, outblk, _AES_BLOCK_LEN);

		encLen += _AES_BLOCK_LEN;
		pClearData +=  _AES_BLOCK_LEN;
		clearLen -= _AES_BLOCK_LEN;
	}
	if(clearLen)
	{
		memcpy (inblk, pClearData, clearLen);
		memset (inblk + clearLen, _AES_BLOCK_LEN - clearLen, _AES_BLOCK_LEN - clearLen);
		AES_encrypt (inblk, outblk, &aesKey);	
	    encData = (unsigned char *) realloc (encData, encLen + _AES_BLOCK_LEN);
		memcpy (encData + encLen, outblk, _AES_BLOCK_LEN);
		encLen += _AES_BLOCK_LEN;
	}
	outputLen = encLen;
	return encData;
    
}
unsigned char* EngineCrypto::AES_ECB_Decrypt(unsigned char*InputString,unsigned int inpLen,unsigned int& outputLen,unsigned char* key,unsigned int keylen) //Encrypt string 
{
	int status = STATUS_OK;
	unsigned char *clearData = NULL;
	unsigned int clearLen;
	unsigned char *encData = InputString;
	unsigned int encLen = inpLen;
	unsigned char *pEncData = encData;
    /*** decriptez AES datele */
	AES_KEY		aesKey;
	status = (AES_set_decrypt_key(key, keylen * 8, &aesKey) != 0);
	if (status != STATUS_OK)
		 throw new std::exception("Eroare setare cheie AES");
	unsigned char inblk[_AES_BLOCK_LEN], outblk[_AES_BLOCK_LEN];
	clearData = (unsigned char *) malloc (0);
	clearLen = 0;
	while (encLen >= _AES_BLOCK_LEN)
	{
		memcpy (inblk, pEncData, _AES_BLOCK_LEN);	
		AES_decrypt (inblk, outblk, &aesKey);
		clearData = (unsigned char *) realloc (clearData, clearLen + _AES_BLOCK_LEN);
		memcpy (clearData + clearLen, outblk, _AES_BLOCK_LEN);
		clearLen += _AES_BLOCK_LEN;
		pEncData +=  _AES_BLOCK_LEN;
		encLen -= _AES_BLOCK_LEN;
	}
	if(encLen)
	{
		memcpy (inblk, pEncData,encLen);
		memset (inblk + encLen, _AES_BLOCK_LEN - encLen, _AES_BLOCK_LEN - encLen);
		AES_decrypt(inblk, outblk, &aesKey);	
	    clearData = (unsigned char *) realloc (clearData, clearLen + _AES_BLOCK_LEN);
		memcpy (clearData + clearLen, outblk, _AES_BLOCK_LEN);
		clearLen += _AES_BLOCK_LEN;
	}
	if(encData)
		free(encData);
	outputLen = clearLen;
	return clearData;
}

void EngineCrypto::AES_ECB_Encrypt(char*InFile,char* OutFile,unsigned char* key,unsigned int keyLen)
{
	int status = STATUS_OK;
	unsigned char * clearData;
	unsigned int clearLen;
	status = Util::_read_from_file(InFile, &clearData, &clearLen);
	if (status != STATUS_OK)
		throw new std::exception("Error Input Output");
    unsigned int encLen=0;
	unsigned char* encData = AES_ECB_Encrypt(clearData,clearLen,encLen,key,keyLen);
	status = Util::_write_to_file(OutFile,encData,encLen);
	if (status != STATUS_OK)
		throw new std::exception("Error Input Output");
   
}
void EngineCrypto::AES_ECB_Decrypt(char*InFile,char* OutFile,unsigned char* key,unsigned int keyLen)
{
	int status = STATUS_OK;
	unsigned char * encData;
	unsigned int encLen;
	status = Util::_read_from_file(InFile, &encData, &encLen);
	if (status != STATUS_OK)
		throw new std::exception("Error Input Output");
    unsigned int clearLen=0;
	unsigned char* clearData = this->AES_ECB_Decrypt(encData,encLen,clearLen,key,keyLen);
	status = Util::_write_to_file(OutFile,clearData,clearLen);
	if (status != STATUS_OK)
		throw new std::exception("Error Input Output");
}

unsigned char* EngineCrypto::AES_CBC_Encrypt(unsigned char*InputString,unsigned int inpLen ,unsigned int& outputLen,unsigned char* key,unsigned int keylen,unsigned char* IV)
{
	int status = STATUS_OK;
	unsigned char *clearData = InputString;
	unsigned int clearLen = inpLen;
	unsigned char *encData = NULL;
	unsigned int encLen;
	unsigned char *pClearData = clearData;
    /*** criptez AES datele */
	AES_KEY		aesKey;
	status = (AES_set_encrypt_key(key, keylen * 8, &aesKey) != 0);
	if (status != STATUS_OK)
		 throw new std::exception("Eroare setare cheie AES");
	unsigned char inblk[_AES_BLOCK_LEN], outblk[_AES_BLOCK_LEN];
	encData = (unsigned char *) malloc (0);
	encLen = 0;
	while (clearLen >= _AES_BLOCK_LEN)
	{
		memcpy (inblk, pClearData, _AES_BLOCK_LEN);	
		
		AES_cbc_encrypt(inblk, outblk,16,&aesKey,IV,AES_ENCRYPT);
			
		encData = (unsigned char *) realloc (encData, encLen + _AES_BLOCK_LEN);
		memcpy (encData + encLen, outblk, _AES_BLOCK_LEN);

		encLen += _AES_BLOCK_LEN;
		pClearData +=  _AES_BLOCK_LEN;
		clearLen -= _AES_BLOCK_LEN;
	}
	if(clearLen)
	{
		memcpy (inblk, pClearData, clearLen);
		memset (inblk + clearLen, _AES_BLOCK_LEN - clearLen, _AES_BLOCK_LEN - clearLen);
		AES_cbc_encrypt(inblk, outblk,16,&aesKey,IV,AES_ENCRYPT);
	    encData = (unsigned char *) realloc (encData, encLen + _AES_BLOCK_LEN);
		memcpy (encData + encLen, outblk, _AES_BLOCK_LEN);
		encLen += _AES_BLOCK_LEN;
	}
	outputLen = encLen;
	return encData;
    
}
unsigned char* EngineCrypto::AES_CBC_Decrypt(unsigned char*InputString,unsigned int inpLen,unsigned int& outputLen,unsigned char* key,unsigned int keylen,unsigned char* IV) //Encrypt string 
{
	int status = STATUS_OK;
	unsigned char *clearData = NULL;
	unsigned int clearLen;
	unsigned char *encData = InputString;
	unsigned int encLen = inpLen;
	unsigned char *pEncData = encData;
    /*** decriptez AES datele */
	AES_KEY		aesKey;
	status = (AES_set_decrypt_key(key, keylen * 8, &aesKey) != 0);
	if (status != STATUS_OK)
		 throw new std::exception("Eroare setare cheie AES");
	unsigned char inblk[_AES_BLOCK_LEN], outblk[_AES_BLOCK_LEN];
	clearData = (unsigned char *) malloc (0);
	clearLen = 0;
	while (encLen >= _AES_BLOCK_LEN)
	{
		memcpy (inblk, pEncData, _AES_BLOCK_LEN);	
		AES_cbc_encrypt(inblk, outblk,16,&aesKey,IV,AES_DECRYPT);
		clearData = (unsigned char *) realloc (clearData, clearLen + _AES_BLOCK_LEN);
		memcpy (clearData + clearLen, outblk, _AES_BLOCK_LEN);
		clearLen += _AES_BLOCK_LEN;
		pEncData +=  _AES_BLOCK_LEN;
		encLen -= _AES_BLOCK_LEN;
	}
	if(encLen)
	{
		memcpy (inblk, pEncData,encLen);
		memset (inblk + encLen, _AES_BLOCK_LEN - encLen, _AES_BLOCK_LEN - encLen);
		AES_cbc_encrypt(inblk, outblk,16,&aesKey,IV,AES_DECRYPT);	
	    clearData = (unsigned char *) realloc (clearData, clearLen + _AES_BLOCK_LEN);
		memcpy (clearData + clearLen, outblk, _AES_BLOCK_LEN);
		clearLen += _AES_BLOCK_LEN;
	}
	if(encData)
		free(encData);
	outputLen = clearLen;
	return clearData;
    
}

unsigned char* EngineCrypto::AES_CTR_Encrypt(unsigned char*InputString,unsigned int inpLen ,unsigned int& outputLen,unsigned char* key,unsigned char* IV)
{
	int status = STATUS_OK;
	unsigned char *clearData = InputString;
	unsigned int clearLen = inpLen;
	unsigned char *encData = NULL;
	unsigned int encLen;
	unsigned char *pClearData = clearData;
    /*** criptez AES datele */
	AES_KEY		aesKey;
	status = (AES_set_encrypt_key(key,128, &aesKey) != 0);
	if (status != STATUS_OK)
		 throw new std::exception("Eroare setare cheie AES");
	unsigned char inblk[_AES_BLOCK_LEN], outblk[_AES_BLOCK_LEN];
	encData = (unsigned char *) malloc (0);
	encLen = 0;
	unsigned char ecount[16];
	memset(&ecount[0],0,16);
	unsigned int num = 0;
	while (clearLen >= _AES_BLOCK_LEN)
	{
		memcpy (inblk, pClearData, _AES_BLOCK_LEN);	
		
		AES_ctr128_encrypt(inblk, outblk,16,&aesKey,IV,&ecount[0],&num);
			
		encData = (unsigned char *) realloc (encData, encLen + _AES_BLOCK_LEN);
		memcpy (encData + encLen, outblk, _AES_BLOCK_LEN);

		encLen += _AES_BLOCK_LEN;
		pClearData +=  _AES_BLOCK_LEN;
		clearLen -= _AES_BLOCK_LEN;
	}
	if(clearLen)
	{
		memcpy (inblk, pClearData, clearLen);
		memset (inblk + clearLen, _AES_BLOCK_LEN - clearLen, _AES_BLOCK_LEN - clearLen);
		AES_ctr128_encrypt(inblk, outblk,16,&aesKey,IV,&ecount[0],&num);
	    encData = (unsigned char *) realloc (encData, encLen + _AES_BLOCK_LEN);
		memcpy (encData + encLen, outblk, _AES_BLOCK_LEN);
		encLen += _AES_BLOCK_LEN;
	}
	outputLen = encLen;
	return encData;
    
}
unsigned char* EngineCrypto::AES_CTR_Decrypt(unsigned char*InputString,unsigned int inpLen,unsigned int& outputLen,unsigned char* key,unsigned char* IV) //Encrypt string 
{
	int status = STATUS_OK;
	unsigned char *clearData = NULL;
	unsigned int clearLen;
	unsigned char *encData = InputString;
	unsigned int encLen = inpLen;
	unsigned char *pEncData = encData;
    /*** decriptez AES datele */
	
	AES_KEY		aesKey;
	status = (AES_set_encrypt_key(key,128, &aesKey) != 0);
	if (status != STATUS_OK)
		 throw new std::exception("Eroare setare cheie AES");
	unsigned char inblk[_AES_BLOCK_LEN], outblk[_AES_BLOCK_LEN];
	clearData = (unsigned char *) malloc (0);
	clearLen = 0;
	unsigned char ecount[16];
	memset(&ecount[0],0,16);
	unsigned int num = 0;
	while (encLen >= _AES_BLOCK_LEN)
	{
		memcpy (inblk, pEncData, _AES_BLOCK_LEN);	
		AES_ctr128_encrypt(inblk, outblk,16,&aesKey,IV,&ecount[0],&num);
		clearData = (unsigned char *) realloc (clearData, clearLen + _AES_BLOCK_LEN);
		memcpy (clearData + clearLen, outblk, _AES_BLOCK_LEN);
		clearLen += _AES_BLOCK_LEN;
		pEncData +=  _AES_BLOCK_LEN;
		encLen -= _AES_BLOCK_LEN;
	}
	if(encLen)
	{
		memcpy (inblk, pEncData,encLen);
		memset (inblk + encLen, _AES_BLOCK_LEN - encLen, _AES_BLOCK_LEN - encLen);
		AES_ctr128_encrypt(inblk, outblk,16,&aesKey,IV,&ecount[0],&num);
	    clearData = (unsigned char *) realloc (clearData, clearLen + _AES_BLOCK_LEN);
		memcpy (clearData + clearLen, outblk, _AES_BLOCK_LEN);
		clearLen += _AES_BLOCK_LEN;
	}
	if(encData)
		free(encData);
	outputLen = clearLen;
	return clearData;
    
}

unsigned char* EngineCrypto::DES_3ECB_Encrypt(unsigned char*InputString,unsigned int inpLen ,unsigned int& outputLen,const unsigned char** keys)
{
	DES_cblock *ks1,*ks2,*ks3;
	ks1=(DES_cblock*)malloc(sizeof(DES_cblock));
	DES_string_to_key((const char*)keys[0],ks1);

	ks2=(DES_cblock*)malloc(sizeof(DES_cblock));
	DES_string_to_key((const char*)keys[1],ks2);

	ks3=(DES_cblock*)malloc(sizeof(DES_cblock));
	DES_string_to_key((const char*)keys[2],ks3);

	DES_key_schedule *schedule1,*schedule2,*schedule3;
	schedule1=(DES_key_schedule *)malloc(sizeof(DES_key_schedule));
	schedule2=(DES_key_schedule *)malloc(sizeof(DES_key_schedule));
	schedule3=(DES_key_schedule *)malloc(sizeof(DES_key_schedule));

	DES_set_key_checked(ks1,schedule1);
	DES_set_key_checked(ks2,schedule2);
	DES_set_key_checked(ks3,schedule3);

	int status = STATUS_OK;
	unsigned char *clearData = InputString;
	unsigned int clearLen = inpLen;
	unsigned char *encData = NULL;
	unsigned int encLen;
	unsigned char *pClearData = clearData;
	unsigned char inblk[_DES_BLOCK_LEN], outblk[_DES_BLOCK_LEN];
	encData = (unsigned char *) malloc (0);
	encLen = 0;
	while (clearLen >= _DES_BLOCK_LEN)
	{
		memcpy (inblk, pClearData, _DES_BLOCK_LEN);	
		
		DES_ecb3_encrypt((DES_cblock*)inblk, (DES_cblock*)outblk,schedule1,schedule2,schedule3,DES_ENCRYPT);
			
		encData = (unsigned char *) realloc (encData, encLen + _DES_BLOCK_LEN);
		memcpy (encData + encLen, outblk, _DES_BLOCK_LEN);

		encLen += _DES_BLOCK_LEN;
		pClearData +=  _DES_BLOCK_LEN;
		clearLen -= _DES_BLOCK_LEN;
	}
	if(clearLen)
	{
		memcpy (inblk, pClearData, clearLen);
		memset (inblk + clearLen, _DES_BLOCK_LEN - clearLen, _DES_BLOCK_LEN - clearLen);
		DES_ecb3_encrypt((DES_cblock*)inblk, (DES_cblock*)outblk,schedule1,schedule2,schedule3,DES_ENCRYPT);
	    encData = (unsigned char *) realloc (encData, encLen + _DES_BLOCK_LEN);
		memcpy (encData + encLen, outblk, _DES_BLOCK_LEN);
		encLen += _DES_BLOCK_LEN;
	}
	outputLen = encLen;
	return encData;


}
unsigned char* EngineCrypto::DES_3ECB_Decrypt(unsigned char*InputString,unsigned int inpLen ,unsigned int& outputLen,const unsigned char** keys)
{
	DES_cblock *ks1,*ks2,*ks3;
	ks1=(DES_cblock*)malloc(sizeof(DES_cblock));
	DES_string_to_key((const char*)keys[0],ks1);

	ks2=(DES_cblock*)malloc(sizeof(DES_cblock));
	DES_string_to_key((const char*)keys[1],ks2);

	ks3=(DES_cblock*)malloc(sizeof(DES_cblock));
	DES_string_to_key((const char*)keys[2],ks3);

	DES_key_schedule *schedule1,*schedule2,*schedule3;
	schedule1=(DES_key_schedule *)malloc(sizeof(DES_key_schedule));
	schedule2=(DES_key_schedule *)malloc(sizeof(DES_key_schedule));
	schedule3=(DES_key_schedule *)malloc(sizeof(DES_key_schedule));

	DES_set_key_checked(ks1,schedule1);
	DES_set_key_checked(ks2,schedule2);
	DES_set_key_checked(ks3,schedule3);
	int status = STATUS_OK;
	unsigned char *clearData = NULL;
	unsigned int clearLen;
	unsigned char *encData = InputString;
	unsigned int encLen = inpLen;
	unsigned char *pEncData = encData;
	unsigned char inblk[_DES_BLOCK_LEN], outblk[_DES_BLOCK_LEN];
	clearData = (unsigned char *) malloc (0);
	clearLen = 0;
	while (encLen >= _DES_BLOCK_LEN)
	{
		memcpy (inblk, pEncData, _DES_BLOCK_LEN);	
		DES_ecb3_encrypt((DES_cblock*)inblk, (DES_cblock*)outblk,schedule1,schedule2,schedule3,DES_DECRYPT);
		clearData = (unsigned char *) realloc (clearData, clearLen + _DES_BLOCK_LEN);
		memcpy (clearData + clearLen, outblk, _DES_BLOCK_LEN);
		clearLen += _DES_BLOCK_LEN;
		pEncData +=  _DES_BLOCK_LEN;
		encLen -= _DES_BLOCK_LEN;
	}
	if(encLen)
	{
		memcpy (inblk, pEncData,encLen);
		memset (inblk + encLen, _DES_BLOCK_LEN - encLen, _DES_BLOCK_LEN - encLen);
		DES_ecb3_encrypt((DES_cblock*)inblk, (DES_cblock*)outblk,schedule1,schedule2,schedule3,DES_DECRYPT);
	    clearData = (unsigned char *) realloc (clearData, clearLen + _DES_BLOCK_LEN);
		memcpy (clearData + clearLen, outblk, _DES_BLOCK_LEN);
		clearLen += _DES_BLOCK_LEN;
	}
	if(encData)
		free(encData);
	outputLen = clearLen;
	return clearData;


}

unsigned char *EngineCrypto::DES_cfb64_Encript(unsigned char *Key, unsigned  char *Msg, unsigned  int size)
{

	static unsigned char*    Res;
	int             n = 0;
	DES_cblock      Key2;
	DES_key_schedule schedule;

	Res = (unsigned char *)malloc(size);

	/* Prepare the key for use with DES_cfb64_encrypt */
	memcpy(Key2, Key, _DES_BLOCK_LEN);
	DES_set_odd_parity(&Key2);
	DES_set_key_checked(&Key2, &schedule);

	/* Encryption occurs here */
	DES_cfb64_encrypt((unsigned char *)Msg, (unsigned char *)Res,
		size, &schedule, &Key2, &n, DES_ENCRYPT);

	return (Res);
}

unsigned char *EngineCrypto::DES_cfb64_Decrypt(unsigned char *Key, unsigned char *Msg, unsigned int size)
{

	static unsigned char*    Res;
	int             n = 0;

	DES_cblock      Key2;
	DES_key_schedule schedule;

	Res = (unsigned char *)malloc(size);

	/* Prepare the key for use with DES_cfb64_encrypt */
	memcpy(Key2, Key, _DES_BLOCK_LEN);
	DES_set_odd_parity(&Key2);
	DES_set_key_checked(&Key2, &schedule);

	/* Decryption occurs here */

	DES_cfb64_encrypt((unsigned char *)Msg, (unsigned char *)Res,
		size, &schedule, &Key2, &n, DES_DECRYPT);

	return (Res);

}

unsigned char* EngineCrypto::DES_cbc_Encrypt(unsigned char*InputString, unsigned int inpLen, unsigned int& outputLen, unsigned char* keys, unsigned char *iv)
{
	DES_cblock *ks1;
	ks1 = (DES_cblock*)malloc(sizeof(DES_cblock));
	DES_string_to_key((const char*)keys, ks1);

	DES_key_schedule *schedule1;
	schedule1 = (DES_key_schedule *)malloc(sizeof(DES_key_schedule));
	DES_set_key_checked(ks1, schedule1);

	int status = STATUS_OK;
	unsigned char *clearData = InputString;
	unsigned int clearLen = inpLen;
	unsigned char *encData = NULL;
	unsigned int encLen;
	unsigned char *pClearData = clearData;
	unsigned char inblk[_DES_BLOCK_LEN], outblk[_DES_BLOCK_LEN];
	encData = (unsigned char *)malloc(0);
	encLen = 0;
	while (clearLen >= _DES_BLOCK_LEN)
	{
		memcpy(inblk, pClearData, _DES_BLOCK_LEN);

		DES_cbc_encrypt(inblk, outblk, _DES_BLOCK_LEN, schedule1, (DES_cblock*)iv, DES_ENCRYPT);

		encData = (unsigned char *)realloc(encData, encLen + _DES_BLOCK_LEN);
		memcpy(encData + encLen, outblk, _DES_BLOCK_LEN);

		encLen += _DES_BLOCK_LEN;
		pClearData += _DES_BLOCK_LEN;
		clearLen -= _DES_BLOCK_LEN;
	}
	if (clearLen)
	{
		memcpy(inblk, pClearData, clearLen);
		memset(inblk + clearLen, _DES_BLOCK_LEN - clearLen, _DES_BLOCK_LEN - clearLen);
		DES_cbc_encrypt(inblk, outblk, _DES_BLOCK_LEN, schedule1, (DES_cblock*)iv, DES_ENCRYPT);
		encData = (unsigned char *)realloc(encData, encLen + _DES_BLOCK_LEN);
		memcpy(encData + encLen, outblk, _DES_BLOCK_LEN);
		encLen += _DES_BLOCK_LEN;
	}
	outputLen = encLen;
	return encData;


}

unsigned char* EngineCrypto::DES_cbc_Decrypt(unsigned char*InputString, unsigned int inpLen, unsigned int& outputLen, unsigned char* keys, unsigned char*iv)
{
	DES_cblock *ks1=0, *ks2=0, *ks3=0;
	ks1 = (DES_cblock*)malloc(sizeof(DES_cblock));
	DES_string_to_key((const char*)keys, ks1);

	DES_key_schedule *schedule1=0, *schedule2=0, *schedule3=0;
	schedule1 = (DES_key_schedule *)malloc(sizeof(DES_key_schedule));

	DES_set_key_checked(ks1, schedule1);

	int status = STATUS_OK;
	unsigned char *clearData = NULL;
	unsigned int clearLen;
	unsigned char *encData = InputString;
	unsigned int encLen = inpLen;
	unsigned char *pEncData = encData;
	unsigned char inblk[_DES_BLOCK_LEN], outblk[_DES_BLOCK_LEN];
	clearData = (unsigned char *)malloc(0);
	clearLen = 0;
	while (encLen >= _DES_BLOCK_LEN)
	{
		memcpy(inblk, pEncData, _DES_BLOCK_LEN);
		//DES_ecb3_encrypt((DES_cblock*)inblk, (DES_cblock*)outblk, schedule1, schedule2, schedule3, DES_DECRYPT);
		DES_cbc_encrypt(inblk, outblk, _DES_BLOCK_LEN, schedule1, (DES_cblock*)iv, DES_DECRYPT);
		clearData = (unsigned char *)realloc(clearData, clearLen + _DES_BLOCK_LEN);
		memcpy(clearData + clearLen, outblk, _DES_BLOCK_LEN);
		clearLen += _DES_BLOCK_LEN;
		pEncData += _DES_BLOCK_LEN;
		encLen -= _DES_BLOCK_LEN;
	}
	if (encLen)
	{
		memcpy(inblk, pEncData, encLen);
		memset(inblk + encLen, _DES_BLOCK_LEN - encLen, _DES_BLOCK_LEN - encLen);
		//DES_ecb3_encrypt((DES_cblock*)inblk, (DES_cblock*)outblk, schedule1, schedule2, schedule3, DES_DECRYPT);
		DES_cbc_encrypt(inblk, outblk, _DES_BLOCK_LEN, schedule1, (DES_cblock*)iv, DES_DECRYPT);
		clearData = (unsigned char *)realloc(clearData, clearLen + _DES_BLOCK_LEN);
		memcpy(clearData + clearLen, outblk, _DES_BLOCK_LEN);
		clearLen += _DES_BLOCK_LEN - encLen;
	}
	if (encData)
		free(encData);
	outputLen = clearLen;
	return clearData;


}


unsigned char* DES_3CBC_Encrypt(unsigned char*InputString, unsigned int inpLen, unsigned int& outputLen, const unsigned char** keys,const char* iv)
{
	DES_cblock *ks1, *ks2, *ks3,*_iv;
	ks1 = (DES_cblock*)malloc(sizeof(DES_cblock));
	DES_string_to_key((const char*)keys[0], ks1);

	_iv = (DES_cblock*)malloc(sizeof(DES_cblock));
	DES_string_to_key(iv, _iv);
	DES_set_odd_parity(_iv);

	ks2 = (DES_cblock*)malloc(sizeof(DES_cblock));
	DES_string_to_key((const char*)keys[1], ks2);

	ks3 = (DES_cblock*)malloc(sizeof(DES_cblock));
	DES_string_to_key((const char*)keys[2], ks3);

	DES_key_schedule *schedule1, *schedule2, *schedule3;
	schedule1 = (DES_key_schedule *)malloc(sizeof(DES_key_schedule));
	schedule2 = (DES_key_schedule *)malloc(sizeof(DES_key_schedule));
	schedule3 = (DES_key_schedule *)malloc(sizeof(DES_key_schedule));

	DES_set_key_checked(ks1, schedule1);
	DES_set_key_checked(ks2, schedule2);
	DES_set_key_checked(ks3, schedule3);

	int status = STATUS_OK;
	unsigned char *clearData = InputString;
	unsigned int clearLen = inpLen;
	unsigned char *encData = NULL;
	unsigned int encLen;
	unsigned char *pClearData = clearData;
	unsigned char inblk[_DES_BLOCK_LEN], outblk[_DES_BLOCK_LEN];
	encData = (unsigned char *)malloc(0);
	encLen = 0;
	while (clearLen >= _DES_BLOCK_LEN)
	{
		memcpy(inblk, pClearData, _DES_BLOCK_LEN);

	//	DES_ecb3_encrypt((DES_cblock*)inblk, (DES_cblock*)outblk, schedule1, schedule2, schedule3, DES_ENCRYPT);
	
		unsigned int olen = 0;
		DES_ede3_cbc_encrypt(inblk, outblk, _DES_BLOCK_LEN, schedule1, schedule2, schedule3, _iv, DES_ENCRYPT);


			encData = (unsigned char *)realloc(encData, encLen + _DES_BLOCK_LEN);
		memcpy(encData + encLen, outblk, _DES_BLOCK_LEN);

		encLen += _DES_BLOCK_LEN;
		pClearData += _DES_BLOCK_LEN;
		clearLen -= _DES_BLOCK_LEN;
	}
	if (clearLen)
	{
		memcpy(inblk, pClearData, clearLen);
		memset(inblk + clearLen, _DES_BLOCK_LEN - clearLen, _DES_BLOCK_LEN - clearLen);
		//DES_ecb3_encrypt((DES_cblock*)inblk, (DES_cblock*)outblk, schedule1, schedule2, schedule3, DES_ENCRYPT);
		unsigned int olen = 0;
		DES_ede3_cbc_encrypt(inblk, outblk, _DES_BLOCK_LEN, schedule1, schedule2, schedule3, _iv, DES_ENCRYPT);

		/*unsigned int olen = 0;
		DES_ede3_cbc_encrypt(inblk, outblk, olen, schedule1, schedule2, schedule3, _iv, DES_ENCRYPT);*/

		encData = (unsigned char *)realloc(encData, encLen + _DES_BLOCK_LEN);
		memcpy(encData + encLen, outblk, _DES_BLOCK_LEN);
		encLen += _DES_BLOCK_LEN;
	}
	outputLen = encLen;
	return encData;


}
unsigned char* DES_3CBC_Decrypt(unsigned char*InputString, unsigned int inpLen, unsigned int& outputLen, const unsigned char** keys,const char* iv)
{
	
	
	DES_cblock *ks1, *ks2, *ks3,*_iv;
	ks1 = (DES_cblock*)malloc(sizeof(DES_cblock));
	DES_string_to_key((const char*)keys[0], ks1);

	_iv = (DES_cblock*)malloc(sizeof(DES_cblock));
	DES_string_to_key(iv, _iv);


	ks2 = (DES_cblock*)malloc(sizeof(DES_cblock));
	DES_string_to_key((const char*)keys[1], ks2);

	ks3 = (DES_cblock*)malloc(sizeof(DES_cblock));
	DES_string_to_key((const char*)keys[2], ks3);

	DES_key_schedule *schedule1, *schedule2, *schedule3;
	schedule1 = (DES_key_schedule *)malloc(sizeof(DES_key_schedule));
	schedule2 = (DES_key_schedule *)malloc(sizeof(DES_key_schedule));
	schedule3 = (DES_key_schedule *)malloc(sizeof(DES_key_schedule));

	DES_set_key_checked(ks1, schedule1);
	DES_set_key_checked(ks2, schedule2);
	DES_set_key_checked(ks3, schedule3);

	int status = STATUS_OK;
	unsigned char *clearData = InputString;
	unsigned int clearLen = inpLen;
	unsigned char *encData = NULL;
	unsigned int encLen;
	unsigned char *pClearData = clearData;
	unsigned char inblk[_DES_BLOCK_LEN], outblk[_DES_BLOCK_LEN];
	encData = (unsigned char *)malloc(0);
	encLen = 0;
	while (clearLen >= _DES_BLOCK_LEN)
	{
		memcpy(inblk, pClearData, _DES_BLOCK_LEN);
		unsigned int olen=0;
		DES_ede3_cbc_encrypt(inblk, outblk,olen, schedule1, schedule2, schedule3,_iv, DES_DECRYPT);
	
			encData = (unsigned char *)realloc(encData, encLen + _DES_BLOCK_LEN);
		memcpy(encData + encLen, outblk, _DES_BLOCK_LEN);

		encLen += _DES_BLOCK_LEN;
		pClearData += _DES_BLOCK_LEN;
		clearLen -= _DES_BLOCK_LEN;
	}
	if (clearLen)
	{
		unsigned int olen = 0;
		memcpy(inblk, pClearData, clearLen);
		memset(inblk + clearLen, _DES_BLOCK_LEN - clearLen, _DES_BLOCK_LEN - clearLen);
		//DES_ecb3_encrypt((DES_cblock*)inblk, (DES_cblock*)outblk, schedule1, schedule2, schedule3, DES_DECRYPT);
		DES_ede3_cbc_encrypt(inblk, outblk, olen, schedule1, schedule2, schedule3, _iv, DES_DECRYPT);

		encData = (unsigned char *)realloc(encData, encLen + _DES_BLOCK_LEN);
		memcpy(encData + encLen, outblk, _DES_BLOCK_LEN);
		encLen += _DES_BLOCK_LEN;
	}
	outputLen = encLen;
	return encData;


}
