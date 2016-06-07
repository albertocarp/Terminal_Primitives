#include "RSAEngineCrypto.h"


RSAEngineCrypto::RSAEngineCrypto(void)
{
}

RSAEngineCrypto::~RSAEngineCrypto(void)
{
}

void RSAEngineCrypto::Generate_Keys(int e_value,int key_length,const char* publicFile,const char* privateFile,KEY_RSA mode)
{
	//deschidem fisierele in modurile write
	BIO * handle_public  = BIO_new_file(publicFile,"w");
	BIO * handle_private = BIO_new_file(privateFile,"w");

	BIGNUM *_BigNumber=BN_new();
	BN_add_word(_BigNumber,e_value);
	RSA *_rsaContext=RSA_new();
	EVP_PKEY *_evpContext =NULL;
	RSA_generate_key_ex(_rsaContext,key_length,_BigNumber,NULL);
	if(mode == KEY_RSA::RSA_MODE)
	{
		PEM_write_bio_RSAPrivateKey(handle_private,_rsaContext,NULL,NULL,0,NULL,NULL); 
		PEM_write_bio_RSAPublicKey(handle_public,_rsaContext); 
	}
	if(mode ==KEY_RSA::EVP_MODE)
	{
		EVP_PKEY_CTX* ctx;
		BigNumber *big = new BigNumber(e_value);
		ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
		EVP_PKEY_CTX_set_rsa_keygen_bits(ctx,key_length);
		EVP_PKEY_CTX_set_rsa_keygen_pubexp(ctx,big->_MyBn());
		EVP_PKEY_keygen_init(ctx);
		EVP_PKEY_keygen(ctx,&_evpContext);
		PEM_write_bio_PrivateKey(handle_private,_evpContext,NULL,NULL,0,NULL,NULL); 
		PEM_write_bio_PUBKEY(handle_public,_evpContext); 
	}
	BIO_free(handle_private);
	BIO_free(handle_public);
}
void RSAEngineCrypto::Generate_Keys(int e_value,int key_length,const char* publicFile,const char* privateFile,const char* password,KEY_RSA mode ,const EVP_CIPHER *chi)
{
	//if no cipher is probvided then use 3DES  
	if(chi == nullptr)
		chi = EVP_des_ede3_cbc();
	BIO * handle_public  = BIO_new_file(publicFile,"w");
	BIO * handle_private = BIO_new_file(privateFile,"w");

	EVP_PKEY *_evpContext = NULL;
	BIGNUM *_BigNumber=BN_new();
	BN_add_word(_BigNumber,e_value);
	RSA *_rsaContext=RSA_new();
	RSA_generate_key_ex(_rsaContext,key_length,_BigNumber,NULL); 


	if (mode == KEY_RSA::RSA_MODE)
	{
		PEM_write_bio_RSAPrivateKey(handle_private, _rsaContext, chi, NULL, 0, NULL, (void*)password);
		PEM_write_bio_RSAPublicKey(handle_public, _rsaContext);
	}
	else if (mode == KEY_RSA::EVP_MODE)
	{
	
		EVP_PKEY_CTX* ctx;
		BigNumber *big = new BigNumber(e_value);
		ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
		EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, key_length);
		EVP_PKEY_CTX_set_rsa_keygen_pubexp(ctx, big->_MyBn());
		EVP_PKEY_keygen_init(ctx);
		EVP_PKEY_keygen(ctx, &_evpContext);
		PEM_write_bio_PrivateKey(handle_private, _evpContext, NULL, NULL, 0, NULL, (void*)password);
		PEM_write_bio_PUBKEY(handle_public, _evpContext);

	}

	BIO_free(handle_private);
	BIO_free(handle_public);
}

RSA* RSAEngineCrypto::Read_Key_Private(const char* In,KEY_RSA mode)
{

	BIO *bio=BIO_new_file(In,"r");
	if(mode == RSA_MODE)
	{
		RSA *rsa=RSA_new();
		PEM_read_bio_RSAPrivateKey(bio,&rsa,NULL,NULL);
		BIO_free(bio);
		return rsa;
	}
	else
	{
		EVP_PKEY * evp =NULL;
		EVP_PKEY* pkey = PEM_read_bio_PrivateKey(bio,&evp,NULL,NULL);
		BIO_free(bio);
		return pkey->pkey.rsa;
	}
}


int passwd_callback(char *pcszBuff, int size, int rwflag, void *pPass)
{
	size_t unPass = strlen((char*)pPass);
	if (unPass > (size_t)size)
		unPass = (size_t)size;
	memcpy(pcszBuff, pPass, unPass);
	return (int)unPass;
}

RSA* RSAEngineCrypto::Read_Key_Private(const char* In,const char* Password,KEY_RSA mode)
{
	OpenSSL_add_all_algorithms();
	BIO *bio = BIO_new_file(In, "r");
	if (mode == RSA_MODE)
	{
		
	
		RSA *rsa = RSA_new();

		rsa = PEM_read_bio_RSAPrivateKey(bio, &rsa, NULL, (void*)Password);
		BIO_free(bio);
		return rsa;

	}
	else
	{
		
		EVP_PKEY* EVP = EVP_PKEY_new();
		BIO *bio = BIO_new_file(In, "r");
		
		EVP = PEM_read_bio_PrivateKey(bio, &EVP, NULL, NULL);
		BIO_free(bio);
		return EVP->pkey.rsa;
	
	}
}

RSA* RSAEngineCrypto::Read_Key_Public(const char* In/*, const char* Password*/, KEY_RSA mode)
{
	OpenSSL_add_all_algorithms();
	BIO *bio = BIO_new_file(In, "r");
	if (mode == RSA_MODE)
	{


		RSA *rsa = RSA_new();
	
		rsa = PEM_read_bio_RSAPublicKey(bio, &rsa, NULL, NULL);
		BIO_free(bio);
		return rsa;

	}
	else
	{

		EVP_PKEY* EVP = EVP_PKEY_new();
		BIO *bio = BIO_new_file(In, "r");

		EVP = PEM_read_bio_PUBKEY(bio, &EVP, NULL, NULL);
		BIO_free(bio);
		return EVP->pkey.rsa;

	}
}

unsigned char* RSAEngineCrypto::Encrypt(unsigned char*InputString, unsigned int inpLen, unsigned int& outputLen, const char* keyFile, KEY_RSA mode)
{
	int status = STATUS_OK;
	unsigned char *clearData = InputString;
	unsigned int clearLen = inpLen;
	unsigned char *encData = NULL;
	unsigned int encLen;
	unsigned char *pClearData = clearData;
	RSA * rsa;
	if (mode == EVP_MODE)
	{
		rsa = Read_Key_Public(keyFile, EVP_MODE);
	}
	else
	{
		rsa = Read_Key_Public(keyFile,RSA_MODE);
	}
	unsigned int _RSA_BLOCK_SIZE = RSA_size(rsa);
	unsigned char* inblk = (unsigned char*)malloc(_RSA_BLOCK_SIZE-11);
	unsigned char*outblk = (unsigned char*)malloc(_RSA_BLOCK_SIZE);
	encData = (unsigned char *)malloc(0);
	encLen = 0;
	while (clearLen >= _RSA_BLOCK_SIZE-11)
	{
		memcpy(inblk, pClearData, _RSA_BLOCK_SIZE-11);
		RSA_public_encrypt(_RSA_BLOCK_SIZE-11 , inblk, outblk, rsa, RSA_PKCS1_PADDING);
		int Newsize = encLen + _RSA_BLOCK_SIZE;
		encData = (unsigned char *)realloc(encData, Newsize);
		memcpy(encData + encLen, outblk, _RSA_BLOCK_SIZE);
		encLen += _RSA_BLOCK_SIZE;
		pClearData += _RSA_BLOCK_SIZE-11;
		clearLen -= _RSA_BLOCK_SIZE-11;
		//memset(inblk,0x00,_RSA_BLOCK_SIZE);
	}
	if (clearLen)
	{
		memcpy(inblk, pClearData, clearLen);
		memset(inblk+clearLen,_RSA_BLOCK_SIZE-11-clearLen,_RSA_BLOCK_SIZE-11-clearLen);
		memset(outblk, 0, _RSA_BLOCK_SIZE / 8 + 1);
		int ret = RSA_public_encrypt(_RSA_BLOCK_SIZE-11, inblk, outblk, rsa, RSA_PKCS1_PADDING);
		encData = (unsigned char *)realloc(encData, encLen + _RSA_BLOCK_SIZE);
		memcpy(encData + encLen, outblk, _RSA_BLOCK_SIZE);
		encLen += _RSA_BLOCK_SIZE;
	}
	outputLen = encLen;
	return encData;
}
void RSAEngineCrypto::Encrypt(const char*InputFile, const char* OutFile, const char* Pubkey, KEY_RSA mode)
{
	int status = STATUS_OK;
	unsigned char * clearData;
	unsigned int clearLen;
	status = Util::_read_from_file((char*)InputFile, &clearData, &clearLen);
	if (status != STATUS_OK)
		throw new std::exception("Error Input Output");
	unsigned int outputLen = 0;
	unsigned char* encData = Encrypt(clearData, clearLen, outputLen, Pubkey, mode);
	status = Util::_write_to_file((char*)OutFile, encData, outputLen);
	if (status != STATUS_OK)
		throw new std::exception("Error Input Output");
}
unsigned char* RSAEngineCrypto::Decrypt(unsigned char*InputString, unsigned int inpLen, unsigned int& outputLen, const char* Privkey, const char* password, KEY_RSA mode)
{
	int status = STATUS_OK;

	unsigned char *clearData = NULL;
	unsigned int clearLen;
	unsigned char *encData = InputString;
	unsigned int encLen = inpLen;
	unsigned char *pEncData = encData;
	RSA * rsa;
	if (password == nullptr)
		if (mode == EVP_MODE)
			rsa = Read_Key_Private(Privkey, EVP_MODE);
		else
			rsa = Read_Key_Private(Privkey,RSA_MODE);
	else
	{
		rsa = Read_Key_Private(Privkey, password, RSA_MODE);
	}
	unsigned int _RSA_BLOCK_SIZE = RSA_size(rsa);
	unsigned char* inblk = (unsigned char*)malloc(_RSA_BLOCK_SIZE);
	unsigned char* outblk = (unsigned char*)malloc(_RSA_BLOCK_SIZE);
	clearData = (unsigned char *)malloc(0);
	clearLen = 0;
	while (encLen >= _RSA_BLOCK_SIZE)
	{
		memcpy(inblk, pEncData, _RSA_BLOCK_SIZE);

		int k=RSA_private_decrypt(_RSA_BLOCK_SIZE, inblk, outblk, rsa, RSA_PKCS1_PADDING);
		clearData = (unsigned char *)realloc(clearData, clearLen + _RSA_BLOCK_SIZE);
		memcpy(clearData + clearLen, outblk, k);
		clearLen += k;
		pEncData += _RSA_BLOCK_SIZE;
		encLen -= _RSA_BLOCK_SIZE;
	}
	if (encLen)
	{
		memcpy(inblk, pEncData, encLen);
		RSA_public_decrypt(encLen, inblk, outblk, rsa, RSA_PKCS1_PADDING);
		clearData = (unsigned char *)realloc(clearData, clearLen + _RSA_BLOCK_SIZE);
		memcpy(clearData + clearLen, outblk, _RSA_BLOCK_SIZE);
		clearLen += _RSA_BLOCK_SIZE;
	}
	if (encData)
		free(encData);
	outputLen = clearLen;
	return clearData;

}
void RSAEngineCrypto::Decrypt(const char*InputFile, const char* OutFile, const char* Privkey, const char* password, KEY_RSA mode)
{
	int status = STATUS_OK;
	unsigned char * encData;
	unsigned int encLen;
	status = Util::_read_from_file((char*)InputFile, &encData, &encLen);
	if (status != STATUS_OK)
		throw new std::exception("Error Input Output");
	unsigned int outputLen = 0;
	encData = Decrypt(encData, encLen, outputLen, Privkey, password, RSA_MODE);
	status = Util::_write_to_file((char*)OutFile, encData, outputLen);
	if (status != STATUS_OK)
		throw new std::exception("Error Input Output");
}
unsigned char* RSAEngineCrypto::Encrypt_Private(unsigned char*InputString, unsigned int inpLen, unsigned int& outputLen, const char* Privkey, KEY_RSA mode)
{
	int status = STATUS_OK;
	unsigned char *clearData = InputString;
	unsigned int clearLen = inpLen;
	unsigned char *encData = NULL;
	unsigned int encLen;
	unsigned char *pClearData = clearData;
	RSA * rsa;
	if (mode == EVP_MODE)
	{
		rsa = Read_Key_Private(Privkey, EVP_MODE);
	}
	else
	{
		rsa = Read_Key_Private(Privkey,RSA_MODE);
	}
	unsigned int _RSA_BLOCK_SIZE = RSA_size(rsa);
	unsigned char* inblk = (unsigned char*)malloc(_RSA_BLOCK_SIZE);
	unsigned char*outblk = (unsigned char*)malloc(_RSA_BLOCK_SIZE);
	encData = (unsigned char *)malloc(0);
	encLen = 0;
	while (clearLen >= _RSA_BLOCK_SIZE)
	{
		memcpy(inblk, pClearData, _RSA_BLOCK_SIZE);
		RSA_private_encrypt(_RSA_BLOCK_SIZE - 11, inblk, outblk, rsa, RSA_PKCS1_PADDING);
		int Newsize = encLen + _RSA_BLOCK_SIZE;
		encData = (unsigned char *)realloc(encData, Newsize);
		memcpy(encData + encLen, outblk, _RSA_BLOCK_SIZE);
		encLen += _RSA_BLOCK_SIZE;
		pClearData += _RSA_BLOCK_SIZE;
		clearLen -= _RSA_BLOCK_SIZE;
	}
	if (clearLen)
	{
		memcpy(inblk, pClearData, clearLen);
		memset(outblk, 0, _RSA_BLOCK_SIZE / 8 + 1);
		int ret = RSA_private_encrypt(_RSA_BLOCK_SIZE - 11, inblk, outblk, rsa, RSA_PKCS1_PADDING);
		encData = (unsigned char *)realloc(encData, encLen + _RSA_BLOCK_SIZE);
		memcpy(encData + encLen, outblk, _RSA_BLOCK_SIZE);
		encLen += _RSA_BLOCK_SIZE;
	}
	outputLen = encLen;
	return encData;
}
void RSAEngineCrypto::Encrypt_Private(const char*InputFile, const char* OutFile, const char* Privkey, KEY_RSA mode)
{
	int status = STATUS_OK;
	unsigned char * clearData;
	unsigned int clearLen;
	status = Util::_read_from_file((char*)InputFile, &clearData, &clearLen);
	if (status != STATUS_OK)
		throw new std::exception("Error Input Output");
	unsigned int outputLen = 0;
	unsigned char* encData = Encrypt_Private(clearData, clearLen, outputLen, Privkey, mode);
	status = Util::_write_to_file((char*)OutFile, encData, outputLen);
	if (status != STATUS_OK)
		throw new std::exception("Error Input Output");
}
unsigned char* RSAEngineCrypto::Decrypt_Public(unsigned char*InputString, unsigned int inpLen, unsigned int& outputLen, const char* Pubkey, const char* password, KEY_RSA mode)
{
	int status = STATUS_OK;

	unsigned char *clearData = NULL;
	unsigned int clearLen;
	unsigned char *encData = InputString;
	unsigned int encLen = inpLen;
	unsigned char *pEncData = encData;
	RSA * rsa;
	if (mode == EVP_MODE)
		rsa = Read_Key_Public(Pubkey, EVP_MODE);
	else
		rsa = Read_Key_Public(Pubkey,RSA_MODE);
	unsigned int _RSA_BLOCK_SIZE = RSA_size(rsa);
	unsigned char* inblk = (unsigned char*)malloc(_RSA_BLOCK_SIZE);
	unsigned char* outblk = (unsigned char*)malloc(_RSA_BLOCK_SIZE);
	clearData = (unsigned char *)malloc(0);
	clearLen = 0;
	while (encLen >= _RSA_BLOCK_SIZE)
	{
		memcpy(inblk, pEncData, _RSA_BLOCK_SIZE);

		RSA_public_decrypt(_RSA_BLOCK_SIZE, inblk, outblk, rsa, RSA_PKCS1_PADDING);
		clearData = (unsigned char *)realloc(clearData, clearLen + _RSA_BLOCK_SIZE);
		memcpy(clearData + clearLen, outblk, _RSA_BLOCK_SIZE);
		clearLen += _RSA_BLOCK_SIZE;
		pEncData += _RSA_BLOCK_SIZE;
		encLen -= _RSA_BLOCK_SIZE;
	}
	if (encLen)
	{
		memcpy(inblk, pEncData, encLen);
		RSA_public_decrypt(_RSA_BLOCK_SIZE, inblk, outblk, rsa, RSA_PKCS1_PADDING);
		clearData = (unsigned char *)realloc(clearData, clearLen + _RSA_BLOCK_SIZE);
		memcpy(clearData + clearLen, outblk, _RSA_BLOCK_SIZE);
		clearLen += _RSA_BLOCK_SIZE;
	}
	if (encData)
		free(encData);
	outputLen = clearLen;
	return clearData;

}
void RSAEngineCrypto::Decrypt_Public(const char*InputFile, const char* OutFile, const char* Privkey, const char* password, KEY_RSA mode)
{
	int status = STATUS_OK;
	unsigned char * encData;
	unsigned int encLen;
	status = Util::_read_from_file((char*)InputFile, &encData, &encLen);
	if (status != STATUS_OK)
		throw new std::exception("Error Input Output");
	unsigned int outputLen = 0;
	encData = Decrypt_Public(encData, encLen, outputLen, Privkey, password, RSA_MODE);
	status = Util::_write_to_file((char*)OutFile, encData, outputLen);
	if (status != STATUS_OK)
		throw new std::exception("Error Input Output");
}

