#include "X509Certificate.h"


X509Certificate::X509Certificate(void)
{
	this->cX509 = X509_new();
	if(!cX509)
	{
		throw new std::exception("Unable to create X509 structure");
	}
}
X509Certificate::~X509Certificate(void)
{
}
void X509Certificate::SetSerialName(string& serial)
{
	ASN1_INTEGER_set(X509_get_serialNumber(this->cX509),atoi(serial.c_str()));
}
void X509Certificate::StartTime(string& st)
{
	X509_gmtime_adj(X509_get_notBefore(this->cX509), atoi(st.c_str()));
}
void X509Certificate::SetEndTime(string& end)
{
	X509_gmtime_adj(X509_get_notAfter(this->cX509), atol(end.c_str()));
}
void X509Certificate::SetPublicKey(EVP_PKEY* key)
{
	X509_set_pubkey(this->cX509, key);
}
void X509Certificate::SetCompany(string& company)
{
	auto * name = X509_get_subject_name(this->cX509);
	X509_NAME_add_entry_by_txt(name, "O",  MBSTRING_ASC, (unsigned char *)(company.c_str()), -1, -1, 0);
}
void X509Certificate::SetCountryCode(string& str)
{
	auto *name=X509_get_subject_name(this->cX509);; 
	X509_NAME_add_entry_by_txt(name,"C",
		MBSTRING_ASC,(unsigned char*)str.c_str(), -1, -1, 0);
}
void X509Certificate::setIssuerName(string& str)
{
	auto *name=X509_get_subject_name(this->cX509);
	X509_set_issuer_name(this->cX509, name);
	 
}// the company who issue the certificate
void X509Certificate::Sign()
{
	EVP_PKEY * pkey = X509_get_pubkey(this->cX509);
	if(!X509_sign(this->cX509, pkey, EVP_sha1()))
    {
        std::cerr << "Error signing certificate." << std::endl;
		X509_free(this->cX509);
    }
}
void X509Certificate::WriteToDisk(const char*pubKey,const char* certFile)
{
	FILE * pkey_file = fopen(pubKey, "wb");
    if(!pkey_file)
    {
        std::cerr << "Unable to open \"key.pem\" for writing." << std::endl;
    }
	int ret = PEM_write_PrivateKey(pkey_file, this->getPubKey(), NULL, NULL, 0, NULL, NULL);
    fclose(pkey_file);
    if(!ret)
    {
        std::cerr << "Unable to write private key to disk." << std::endl;
    }
    FILE * x509_file = fopen("cert.pem", "wb");
    if(!x509_file)
    {
        std::cerr << "Unable to open \"cert.pem\" for writing." << std::endl;
    }
	ret = PEM_write_X509(x509_file, this->cX509);
    fclose(x509_file);
    
    if(!ret)
    {
        std::cerr << "Unable to write certificate to disk." << std::endl;
    }
  
}
string& X509Certificate::getSerial()
{
	auto asn_int = X509_get_serialNumber(this->cX509);
	Integer *integer = new Integer(asn_int);
	return Util::ToString(integer->ToLong());
}
EVP_PKEY * X509Certificate::getPubKey()
{
	return X509_get_pubkey(this->cX509);
}