#include "ServerAuth.h"

using namespace BioAuth;
ServerAuth* ServerAuth::p_Instance;

ServerAuth::ServerAuth()
{
}


ServerAuth::~ServerAuth()
{
}


void  BioAuth::ServerAuth::getRegistrationMessage()
{
	char* data = (char*)malloc(SIZE_P + SIZE_Q + HASH_SIZE);
	char* temp = (char*)malloc(200);
	size_t offset = 0,bigOffset=0;
	int x_len;
	p.Generate_prime_BigNumber(1024);
	q.Generate_prime_BigNumber(1024);
	x = BigNumber::fromRandom(x_size);
	unsigned char* x_string = x.ToString(x_len);

	// alegem x din Zq
	memcpy(temp+offset, this->ID, this->size_ID);
	offset += size_ID;
	memcpy(temp + offset, (char*)x_string, x_len);
	offset += x_len;
	temp = (char*)Hash::sha1_v1((unsigned char*)temp,offset,0);
	S1 = (char*)malloc(HASH_SIZE);
	memcpy(S1, temp, HASH_SIZE);
	printf("S1=\n");
	Helper::print_hexa(S1, HASH_SIZE);
	// copy S1 to final message;
	memcpy(data,temp+bigOffset, HASH_SIZE);
	bigOffset += HASH_SIZE;
	offset = 0;


	memcpy(temp + offset, this->hash_R, HASH_SIZE);
	offset += HASH_SIZE;
	memcpy(temp + offset, this->hash_R_PW, HASH_SIZE);
	offset += HASH_SIZE;
	temp=(char*)Hash::sha1_v1((unsigned char*)temp, offset, 0);
	printf("S2=\n");
	Helper::print_hexa(temp, HASH_SIZE);

	// calcultates s3 and store it in data
	Helper:: xor (data, temp, HASH_SIZE);

	printf("S3=\n");
	Helper::print_hexa(data, HASH_SIZE);

	unsigned char* p = (unsigned char*)_strdup((char*)GROUP_P_LOCAL);// this->p.ToString();
	unsigned char* q = this->q.ToString();

	memcpy(data+ bigOffset, p, SIZE_P);
	bigOffset += SIZE_P;
	memcpy(data+ bigOffset, q, SIZE_Q);
	bigOffset += SIZE_Q;

   // know sends the message to the client
	word chunk_data = 127;
	size_t chunk_offset = bigOffset;
	size_t chunk_index = 0;
	char* temp_data = (char*)malloc(chunk_data);
	while (chunk_offset > chunk_data)
	{
		Apdu*apdu = new ApduRequest(Configuration::APPLICATION_CLA, Configuration::OFFSET_INS_BIOMETRIC,
			OFFSET_PI_SECOND_MESSAGE,OFFSET_P2_MORE_FRAGMENTS, chunk_data, 0x00);
		memcpy(temp_data,data + chunk_index, chunk_data);
		apdu->wrap((byte*)temp_data, chunk_data);
		Apdu& response = this->card.sendApdu(*apdu);
		std::cout << response.toString();
		chunk_offset -= chunk_data;
		chunk_index += chunk_data;
		delete apdu;
	}
	if (chunk_offset > 0)
	{
		// we have un block of size less than block_size
		Apdu*apdu = new ApduRequest(Configuration::APPLICATION_CLA, Configuration::OFFSET_INS_BIOMETRIC,
			OFFSET_PI_SECOND_MESSAGE, OFFSET_P2_LAST_FRAGMENT, chunk_offset, 0x00);
		memcpy(temp_data,data + chunk_index, chunk_offset);
		apdu->wrap((byte*)temp_data, chunk_offset);
		Apdu& response = this->card.sendApdu(*apdu);
		std::cout << response.toString();
		delete apdu;
	}
	free(temp_data);
	free(temp);
	free(data);
	free(p);
	free(q);
}

void BioAuth::ServerAuth::setXSize(size_t x)
{
	this->x_size = x;
}
void BioAuth::ServerAuth::getP()
{
	printf("P=\n");
	byte dummy[] = { 0x00 };
	Apdu* apdu = new ApduRequest(Configuration::APPLICATION_CLA, Configuration::OFFSET_INS_BIOMETRIC,
		OFFSET_P1_GET_P, 0x00, 0x01, 0x00);
	apdu->wrap(dummy, 1);
	Apdu& response = this->card.sendApdu(*apdu);
	std::cout << response.toString();

	delete apdu;
}
void BioAuth::ServerAuth::getQ()
{
	printf("Q=\n");
	byte dummy[] = { 0x00 };
	Apdu* apdu = new ApduRequest(Configuration::APPLICATION_CLA, Configuration::OFFSET_INS_BIOMETRIC,
		OFFSET_P1_GET_Q, 0x00, 0x01, 0x00);
	apdu->wrap(dummy, 1);
	Apdu& response = this->card.sendApdu(*apdu);
	std::cout << response.toString();
	delete apdu;
}
void BioAuth::ServerAuth::getS3()
{
	printf("S3=\n");
	byte dummy[] = { 0x00 };
	Apdu* apdu = new ApduRequest(Configuration::APPLICATION_CLA, Configuration::OFFSET_INS_BIOMETRIC,
		OFFSET_P1_GET_S3, 0x00, 0x01, 0x00);
	apdu->wrap(dummy, 1);
	Apdu& response = this->card.sendApdu(*apdu);
	std::cout << response.toString();
	delete apdu;
}
void BioAuth::ServerAuth::getS1()
{
	printf("S1=\n");
	byte dummy[] = { 0x00 };
	Apdu* apdu = new ApduRequest(Configuration::APPLICATION_CLA, Configuration::OFFSET_INS_BIOMETRIC,
		OFFSET_P1_GET_S1, 0x00, 0x01, 0x00);
	apdu->wrap(dummy, 1);
	Apdu& response = this->card.sendApdu(*apdu);
	std::cout << response.toString();
	delete apdu;
}
ServerAuth* BioAuth::ServerAuth::getInstance()
{
	if (ServerAuth::p_Instance == nullptr)
	{
		p_Instance = new ServerAuth();
	}
	return p_Instance;
}
