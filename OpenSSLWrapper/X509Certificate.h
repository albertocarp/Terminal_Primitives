#pragma once
#include "Util.h"
#include "ASN1.h"
#include "RSAEngineCrypto.h"
typedef map<string,string> Propreties;
class X509Certificate
{
	X509* cX509;
	Propreties Prop;
public:
	void SetSerialName(string&);
	void StartTime(string&);
	void SetEndTime(string&);
	void SetPublicKey(EVP_PKEY*);
	void SetCompany(string&);
	void SetCountryCode(string&);
	void setIssuerName(string&); // the company who issue the certificate
	void Sign();
	void WriteToDisk(const char*pubKey,const char* certFile);
	string& getSerial();
	//string& getStart();
	//string& getEnd();
	EVP_PKEY * getPubKey();

	X509Certificate(void);
	~X509Certificate(void);
};

