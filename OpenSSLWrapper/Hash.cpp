#include "Hash.h"


Hash::Hash()
{
}
Hash::~Hash()
{
}
uch* Hash::sha512_v1(	uch*  string,	uint strlen,								 uint hex_format )
{
	uch* digest = (uch*)malloc(SHA512_DIGEST_LENGTH);

	SHA512((uch *)string, strlen, digest);
	if (hex_format == 1)
	{
		char* mdString=new char[SHA512_DIGEST_LENGTH * 2 + 1];
		for (int i = 0; i < SHA512_DIGEST_LENGTH; i++)
			sprintf(&mdString[i * 2], "%02x", (uint)digest[i]);
		
		return (uch*)mdString;
	}
	uch* retValue = (uch*)malloc(SHA512_DIGEST_LENGTH);
	memcpy(retValue,digest,SHA512_DIGEST_LENGTH);
	return retValue;


};
uch* Hash::sha512_v2(	uch*  string,	uint strlen,								 uint hex_format)
{
	uch* digest=(uch*)malloc(SHA512_DIGEST_LENGTH);
	SHA512_CTX ctx;
	SHA512_Init(&ctx);
	SHA512_Update(&ctx, string, strlen);
	SHA512_Final(digest, &ctx);

	if (hex_format == 1){
		char* mdString = new char[SHA512_DIGEST_LENGTH * 2 + 1];
		for (int i = 0; i < SHA512_DIGEST_LENGTH; i++)
			sprintf(&mdString[i * 2], "%02x", (uint)digest[i]);

		return (uch*)mdString;
	}
	uch* retValue = (uch*)malloc(SHA512_DIGEST_LENGTH);
	memcpy(retValue,digest,SHA512_DIGEST_LENGTH);
	return retValue;
}
uch* Hash::hmac_v1(		uch*  key,		uint keyLen, uch*data,uint dataLen,			 uint hex_format )
{

	uch* digest;
	
	// Using sha1 hash engine here.
	// You may use other hash engines. e.g EVP_md5(), EVP_sha224, EVP_sha512, etc
	digest = HMAC(EVP_sha1(), key, keyLen, (uch*)data, dataLen, NULL, NULL);

	// Be careful of the length of string with the choosen hash engine. SHA1 produces a 20-byte hash value which rendered as 40 characters.
	// Change the length accordingly with your choosen hash engine
	if (hex_format == 1){
		char* mdString = new char[500*2 + 1];
		for (int i = 0; i < 500; i++)
			sprintf(&mdString[i * 2], "%02x", (uint)digest[i]);

		return (uch*)mdString;
	}

	return digest;

};
uch* Hash::hmac_v2(		uch*  key,		uint keyLen, uch*data,uint dataLen,			 uint hex_format )
{

	// Be careful of the length of string with the choosen hash engine. SHA1 needed 20 characters.
	// Change the length accordingly with your choosen hash engine.     
	uch* result;
	uint len = 20;

	result = (unsigned char*)malloc(sizeof(char) * len);
	
	HMAC_CTX *ctx = new HMAC_CTX;
	HMAC_CTX_init(ctx);

	// Using sha1 hash engine here.
	// You may use other hash engines. e.g EVP_md5(), EVP_sha224, EVP_sha512, etc
	HMAC_Init_ex(ctx, key, keyLen, EVP_sha1(), NULL);
	HMAC_Update(ctx, (unsigned char*)&data, dataLen);
	HMAC_Final(ctx, result, &len);
	HMAC_CTX_cleanup(ctx);
	

	//char mdString[200];
	//for (int i = 0; i < 20; i++)
	//	sprintf(&mdString[i * 2], "%02x", (unsigned int)result[i]);
	//return mdString;


	if (hex_format == 1) {
		char* mdString = new char[500 * 2 + 1];
		for (int i = 0; i < SHA512_DIGEST_LENGTH; i++)
			sprintf(&mdString[i * 2], "%02x", (uint)result[i]);

		return result;
	}
	return 0;
};
uch* Hash::sha256_v1(	uch*  string,	uint strlen, uint hex_format )
{

	uch* digest=(uch*)malloc(SHA256_DIGEST_LENGTH);
	SHA256(string, strlen,digest);
	if (hex_format == 1){
		char* mdString = new char[SHA256_DIGEST_LENGTH * 2 + 1];
		for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
			sprintf(&mdString[i * 2], "%02x", (uint)digest[i]);

		return (uch*)mdString;
	}
	uch* retValue = (uch*)malloc(SHA256_DIGEST_LENGTH);
	memcpy(retValue,digest,SHA256_DIGEST_LENGTH);
	return retValue;

};
uch* Hash::sha256_v2(	uch*  string,	uint strlen,								 uint hex_format )
{

	uch* digest=new uch[SHA256_DIGEST_LENGTH];
	SHA256_CTX ctx;
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, string, strlen);
	SHA256_Final(digest, &ctx);

	
	if (hex_format == 1){
		char* mdString = new char[SHA256_DIGEST_LENGTH * 2 + 1];
		for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
			sprintf(&mdString[i * 2], "%02x", (uint)digest[i]);

		return (uch*)mdString;
	}
	uch* retValue = (uch*)malloc(SHA256_DIGEST_LENGTH);
	memcpy(retValue,digest,SHA256_DIGEST_LENGTH);
	return retValue;


};
uch* Hash::sha1_v1(		uch*  string,	uint strlen,								 uint hex_format )
{

	uch* digest =  (uch*)malloc(SHA_DIGEST_LENGTH);
	SHA1(string, strlen,digest);

	if (hex_format == 1){
		char* mdString = new char[SHA_DIGEST_LENGTH * 2 + 1];
		for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
			sprintf(&mdString[i * 2], "%02x", (uint)digest[i]);

		return (uch*)mdString;
	}
	uch* retValue = (uch*)malloc(SHA_DIGEST_LENGTH);
	memcpy(retValue,digest,SHA_DIGEST_LENGTH);
	free(digest);
	return retValue;


}
uch* Hash::sha1_v2(		uch*  string,	uint strlen,								 uint hex_format )
{
	uch* digest =  (uch*)malloc(SHA_DIGEST_LENGTH);
	SHA_CTX ctx;
	SHA1_Init(&ctx);
	SHA1_Update(&ctx, string, strlen);
	SHA1_Final(digest, &ctx);

	if (hex_format == 1){
		char* mdString = new char[SHA_DIGEST_LENGTH * 2 + 1];
		for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
			sprintf(&mdString[i * 2], "%02x", (uint)digest[i]);

		return (uch*)mdString;
	}
	uch* retValue = (uch*)malloc(SHA_DIGEST_LENGTH);
	memcpy(retValue,digest,SHA_DIGEST_LENGTH);
	return retValue;
};