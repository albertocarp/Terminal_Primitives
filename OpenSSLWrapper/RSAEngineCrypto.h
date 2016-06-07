#pragma once
#include "Util.h"
#include "BigNumber.h"
enum KEY_RSA {
	EVP_MODE,
	RSA_MODE
};
class RSAEngineCrypto
{
private:
	static int getpassphrase_callback(char *buf, int buflen, int verify, void *app_context)
	{
		(void) verify;
		const char *phrase = (const char *)app_context;
		if (buflen < (int)strlen(phrase)+1)
			return -1;
		strcpy(buf, phrase);
		return strlen(phrase);
	}
 
public:
	//Generez chei RSA private/public fara parola
	void Generate_Keys(int e_value,int key_length,const char* publicFile,const char* privateFile,KEY_RSA rsa_style = RSA_MODE); // generare e_value key_length
	
	//generez chei RSA private/public cu parola criptata cu 3DES
	void Generate_Keys(int e_value, int key_length, const char* publicFile, const char* privateFile, const char* password, KEY_RSA mode,const EVP_CIPHER * chichi = nullptr);
	
	//generare pub/priv file  cheia criptate cu parola nu exista PCKS
	RSA* Read_Key_Private(const char* In,KEY_RSA = RSA_MODE);
	RSA* Read_Key_Private(const char* In, const char* Password, KEY_RSA mode);
	RSA* Read_Key_Public(const char* In,KEY_RSA = RSA_MODE);
	
	unsigned char* Encrypt(unsigned char*InputString,unsigned int inpLen ,unsigned int& outputLen,const char* Pubkey,KEY_RSA mode);
	void Encrypt(const char*InputFile,const char* OutFile,const char* Pubkey,KEY_RSA mode);

	unsigned char* Decrypt(unsigned char*InputString,unsigned int inpLen ,unsigned int& outputLen,const char* Pubkey,const char* password = nullptr,KEY_RSA mode = RSA_MODE);
	void Decrypt(const char*InputFile,const char* OutFile,const char* Privkey,const char* password = nullptr,KEY_RSA mode = RSA_MODE);

	unsigned char* Encrypt_Private(unsigned char*InputString,unsigned int inpLen ,unsigned int& outputLen,const char* Privkey,KEY_RSA mode);
	void Encrypt_Private(const char*InputFile,const char* OutFile,const char* Privkey,KEY_RSA mode); // sign

	unsigned char* Decrypt_Public(unsigned char*InputString,unsigned int inpLen ,unsigned int& outputLen,const char* Pubkey,const char* password = nullptr,KEY_RSA mode = RSA_MODE);
	void Decrypt_Public(const char*InputFile,const char* OutFile,const char* Privkey,const char* password = nullptr,KEY_RSA mode = RSA_MODE);

	unsigned char*DES_3CBC_Encrypt(unsigned char*InputString, unsigned int inpLen, unsigned int& outputLen, const unsigned char** keys, const char* iv);
	unsigned char*DES_3CBC_Decrypt(unsigned char*InputString, unsigned int inpLen, unsigned int& outputLen, const unsigned char** keys, const char* iv);
	RSAEngineCrypto(void);
	~RSAEngineCrypto(void);
};

