// ***********************************************************************
// Assembly         : 
// Author           : Alberto-PC
// Created          : 05-30-2016
//
// Last Modified By : Alberto-PC
// Last Modified On : 06-08-2016
// ***********************************************************************
// <copyright file="ClientAuth.cpp" company="Militarcy Technical Academy">
//     Copyright (c) . All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************
#include "ClientAuth.h"

using namespace BioAuth;

Config* Config::p_Instance;
/// <summary>
/// The p instance
/// </summary>
/// The instance of the current class
ClientAuth* ClientAuth::pInstance;

/// <summary>
/// Initializes a new instance of the <see cref="ClientAuth"/> class.
/// </summary>
/// <param name="card">The card.</param>
ClientAuth::ClientAuth(const Card& card)
{
	this->card = card;
	Config::Instance();
}

/// <summary>
/// Prevents a default instance of the <see cref="ClientAuth"/> class from being created.
/// </summary>
ClientAuth::ClientAuth()
{
	Config::Instance();
}

/// <summary>
/// Gets the password.
/// </summary>
/// <returns>char *.</returns>
char * BioAuth::ClientAuth::getPassword()
{
	char dummy[] = { 0x00 };
	Apdu* apdu = new ApduRequest(Configuration::APPLICATION_CLA, Configuration::OFFSET_INS_BIOMETRIC,
		OFFSET_P1_GET_PASSWORD, 0x00, 0x01, 0x00);
	apdu->wrap((byte*)dummy, 1);
	Apdu& response = this->card.sendApdu(*apdu);
#ifdef LOG
	std::cout << response.toString();
#endif
	delete apdu;
	return (char*)response.toString().c_str();
}

/// <summary>
/// Gets the data.
/// </summary>
void BioAuth::ClientAuth::getData()
{
	char dummy[] = { 0x00 };
	Apdu* apdu = new ApduRequest(Configuration::APPLICATION_CLA, Configuration::OFFSET_INS_BIOMETRIC,
		OFFSET_P1_GET_ADITIONAL_FIELDS, 0x00, 0x01, 0x00);
	apdu->wrap((byte*)dummy, 1);
	Apdu& response = this->card.sendApdu(*apdu);
#ifdef LOG
	std::cout << response.toString();
#endif
	delete apdu;
}

/// <summary>
/// Gets the galois.
/// </summary>
void BioAuth::ClientAuth::getGalois()
{
	char dummy[] = { 0x00 };
	Apdu* apdu = new ApduRequest(Configuration::APPLICATION_CLA, Configuration::OFFSET_INS_BIOMETRIC,
		OFFSET_P1_GET_GALOIS, 0x00, 0x01, 0x00);
	apdu->wrap((byte*)dummy, 1);
	Apdu& response = this->card.sendApdu(*apdu);
#ifdef LOG
	std::cout << response.toString();
#endif
	delete apdu;
}

/// <summary>
/// Gets the biometrics.
/// </summary>
void BioAuth::ClientAuth::getBiometrics()
{
	char dummy[] = { 0x00 };
	Apdu* apdu = new ApduRequest(Configuration::APPLICATION_CLA, Configuration::OFFSET_INS_BIOMETRIC,
		OFFSET_P1_GET_BIO, 0x00, 0x01, 0x00);
	apdu->wrap((byte*)dummy, 1);
	Apdu& response = this->card.sendApdu(*apdu);
#ifdef LOG
	std::cout << response.toString();
#endif
	delete apdu;
}

/// <summary>
/// Gets the sk.
/// </summary>
void BioAuth::ClientAuth::getSk()
{
	char dummy[] = { 0x00 };
	Apdu* apdu = new ApduRequest(Configuration::APPLICATION_CLA, Configuration::OFFSET_INS_BIOMETRIC,
		OFFSET_P1_GET_SK, 0x00, 0x01, 0x00);
	apdu->wrap((byte*)dummy, 1);
	Apdu& response = this->card.sendApdu(*apdu);
#ifdef LOG
	printf("Sk=\n");
	std::cout << response.toString();
#endif
	delete apdu;
}

/// <summary>
/// Gets the s1.
/// </summary>
void BioAuth::ClientAuth::getS1()
{

}

/// <summary>
/// Finalizes an instance of the <see cref="ClientAuth"/> class.
/// </summary>
ClientAuth::~ClientAuth()
{

}

/// <summary>
/// Sets the identifier.
/// </summary>
/// <param name="id">The identifier.</param>
/// <param name="size">The size.</param>
void ClientAuth::setId(byte* id, size_t size)
{
	this->id.data = (byte*)malloc(size);
	memcpy(this->id.data, id, size);
	this->id.data_size = size;
}

/// <summary>
/// Sets the password.
/// </summary>
/// <param name="password">The password.</param>
/// <param name="size">The size.</param>
void ClientAuth::setPassword(byte* password, size_t size)
{
	this->password.data = (byte*)malloc(size);
	memcpy(this->password.data, password, size);
	this->password.data_size = size;
}

/// <summary>
/// Sets the biometrics information.
/// </summary>
/// <param name="biometrics">The biometrics.</param>
/// <param name="size">The size.</param>
void ClientAuth::setBiometricsInfo(byte* biometrics, size_t size)
{
	this->biometrics.data = (byte*)malloc(size);
	memcpy(this->biometrics.data, biometrics, size);
	this->biometrics.data_size = size;
}

/// <summary>
/// Stores the data.
/// </summary>
void ClientAuth::storeData()
{
	// sends the password
	Apdu* apdu = new ApduRequest(Configuration::APPLICATION_CLA, Configuration::OFFSET_INS_BIOMETRIC,
		OFFSET_P1_SET_PASSWORD,0x00, (byte)this->password.data_size, 0x00);

	apdu->wrap(this->password.data, this->password.data_size);
	Apdu& response = this->card.sendApdu(*apdu);
#ifdef LOG
	std::cout<<response.toString();
#endif
	delete apdu;
	// sends the identity
	apdu = new ApduRequest(Configuration::APPLICATION_CLA, Configuration::OFFSET_INS_BIOMETRIC,
		(byte)OFFSET_P1_SET_IDENTITY,0x00,(byte)this->id.data_size, 0x00);

	apdu->wrap(this->id.data, this->id.data_size);
	response = this->card.sendApdu(*apdu);
#ifdef LOG
	std::cout << response.toString();
#endif
	delete apdu;
	// know sends the biometric info
	byte chunk_data = 126;
	size_t chunk_offset = this->biometrics.data_size;
	size_t chunk_index = 0;
	byte *data = (byte*)malloc(chunk_data);
	while (chunk_offset > chunk_data)
	{
		apdu = new ApduRequest(Configuration::APPLICATION_CLA, Configuration::OFFSET_INS_BIOMETRIC,
			OFFSET_P1_CHAIN_BIO,(byte)0x00,chunk_data, 0x00);
		memcpy(data, this->biometrics.data + chunk_index, chunk_data);
		apdu->wrap(data, chunk_data);
		response = this->card.sendApdu(*apdu);
#ifdef LOG
		std::cout << response.toString();
#endif
		chunk_offset -= chunk_data;
		chunk_index += chunk_data;
		delete apdu;
	}
	if (chunk_offset > 0)
	{
		// we have un block of size less than block_size
		apdu = new ApduRequest(Configuration::APPLICATION_CLA, Configuration::OFFSET_INS_BIOMETRIC,
			OFFSET_P1_CHAIN_BIO,0x00, (byte)chunk_offset, 0x00);
		memcpy(data, this->biometrics.data + chunk_index, chunk_offset);
		apdu->wrap((byte*)data, chunk_offset);
#ifdef LOG
	    response = this->card.sendApdu(*apdu);
#endif
		std::cout << response.toString();
		delete apdu;
	}

	// store config aditional data
	/*
	apdu = new ApduRequest(Configuration::APPLICATION_CLA, Configuration::OFFSET_INS_BIOMETRIC,
		OFFSET_P1_ADDITIONAL_DATA,OFFSET_P2_CONFIG_DATA,6, 0x00);

	apdu->wrap((byte*)getConfigData(), 6);
	response = this->card.sendApdu(*apdu);
	std::cout << response.toString();
	*/
	GaloisField galois(M_SHORT,LENGTH_SHORT,T_SHORT);
	short* galoisPolinom = galois.generatePolinom();
	int rdncy = galois.getLastRedundancy();
	chunk_offset = rdncy *sizeof(short);
	chunk_index = 0;
	while (chunk_offset > chunk_data)
	{
		apdu = new ApduRequest(Configuration::APPLICATION_CLA, Configuration::OFFSET_INS_BIOMETRIC,
			OFFSET_P1_ADDITIONAL_DATA, OFFSET_P2_GALOIS_FIELD, chunk_data, 0x00);
		memcpy(data, galoisPolinom + (chunk_index/2), chunk_data);
		apdu->wrap(data, chunk_data);
		response = this->card.sendApdu(*apdu);
#ifdef LOG
		std::cout << response.toString();
#endif
		chunk_offset -= chunk_data;
		chunk_index += chunk_data;
		delete apdu;
	}
	if (chunk_offset > 0)
	{
		// we have un block of size less than block_size
		apdu = new ApduRequest(Configuration::APPLICATION_CLA, Configuration::OFFSET_INS_BIOMETRIC,
			OFFSET_P1_ADDITIONAL_DATA, OFFSET_P2_GALOIS_FIELD, (byte)chunk_offset, 0x00);
		memcpy(data, galoisPolinom + (chunk_index/2), chunk_offset);
		apdu->wrap((byte*)data, chunk_offset);
		response = this->card.sendApdu(*apdu);
#ifdef LOG
		std::cout << response.toString();
#endif
		delete apdu;
	}
	delete galoisPolinom;
}

/// <summary>
/// Stores the authentication data.
/// </summary>
void BioAuth::ClientAuth::storeAuthData()
{
	// sends the password
	Apdu* apdu = new ApduRequest(Configuration::APPLICATION_CLA, Configuration::OFFSET_INS_BIOMETRIC,
		OFFSET_P1_AUTH_PASSWORD, 0x00, (byte)this->password.data_size, 0x00);

	apdu->wrap(this->password.data, this->password.data_size);
	Apdu& response = this->card.sendApdu(*apdu);
#ifdef LOG
	std::cout << response.toString();
#endif
	delete apdu;
	// sends the identity
	apdu = new ApduRequest(Configuration::APPLICATION_CLA, Configuration::OFFSET_INS_BIOMETRIC,
		(byte)OFFSET_P1_AUTH_IDENTITY, 0x00, (byte)this->id.data_size, 0x00);

	apdu->wrap(this->id.data, this->id.data_size);
	response = this->card.sendApdu(*apdu);
#ifdef LOG
	std::cout << response.toString();
#endif
	delete apdu;
	// know sends the biometric info
	byte chunk_data = 126;
	size_t chunk_offset = this->biometrics.data_size;
	size_t chunk_index = 0;
	byte *data = (byte*)malloc(chunk_data);
	while (chunk_offset > chunk_data)
	{
		apdu = new ApduRequest(Configuration::APPLICATION_CLA, Configuration::OFFSET_INS_BIOMETRIC,
			OFFSET_P1_CHAIN_AUTH, (byte)0x00, chunk_data, 0x00);
		memcpy(data, this->biometrics.data + chunk_index, chunk_data);
		apdu->wrap(data, chunk_data);
		response = this->card.sendApdu(*apdu);
#ifdef LOG
		std::cout << response.toString();
#endif
		chunk_offset -= chunk_data;
		chunk_index += chunk_data;
		delete apdu;
	}
	if (chunk_offset > 0)
	{
		// we have un block of size less than block_size
		apdu = new ApduRequest(Configuration::APPLICATION_CLA, Configuration::OFFSET_INS_BIOMETRIC,
			OFFSET_P1_CHAIN_AUTH, 0x00, (byte)chunk_offset, 0x00);
		memcpy(data, this->biometrics.data + chunk_index, chunk_offset);
		apdu->wrap((byte*)data, chunk_offset);
		response = this->card.sendApdu(*apdu);
#ifdef LOG
		std::cout << response.toString();
#endif
		delete apdu;
	}

	// store config aditional data
	/*
	apdu = new ApduRequest(Configuration::APPLICATION_CLA, Configuration::OFFSET_INS_BIOMETRIC,
	OFFSET_P1_ADDITIONAL_DATA,OFFSET_P2_CONFIG_DATA,6, 0x00);

	apdu->wrap((byte*)getConfigData(), 6);
	response = this->card.sendApdu(*apdu);
	std::cout << response.toString();
	*/
}

/// <summary>
/// Registrations this instance.
/// </summary>
/// <returns>BioAuth.data_size_structure&lt;type&gt;.</returns>
data_size_structure<char*> BioAuth::ClientAuth::registration()
{
	byte dummy[] = { 0x00 };
	Apdu* apdu = new ApduRequest(Configuration::APPLICATION_CLA, Configuration::OFFSET_INS_BIOMETRIC,
		OFFSET_P1_ADDITIONAL_DATA, OFFSET_P2_COMPUTE, 0x01, 0x00);
	apdu->wrap(dummy, 1);
	Apdu& response = this->card.sendApdu(*apdu);
#ifdef LOG
	std::cout << response.toString();
#endif
	delete apdu;
	apdu = new ApduRequest(Configuration::APPLICATION_CLA, Configuration::OFFSET_INS_BIOMETRIC,
		OFFSET_P1_REGISTRATION, 0x00, 0x01, 0x00);
	apdu->wrap(dummy, 1);
	response = this->card.sendApdu(*apdu);
#ifdef LOG
	std::cout << response.toString();
#endif
	delete apdu;
	apdu = new ApduRequest(Configuration::APPLICATION_CLA, Configuration::OFFSET_INS_BIOMETRIC,
		OFFSET_P1_REGISTRATION_MESSAGE, 0x00, 0x01, 0x00);
	apdu->wrap(dummy, 1);
	response = this->card.sendApdu(*apdu);
#ifdef LOG
	std::cout << response.toString();
#endif
	delete apdu;
	data_size_structure<char*>* data = new data_size_structure<char*>();
	data->data = (char*)malloc(response.getDataLength());
	memcpy(data->data, response.getData(), response.getDataLength());
	data->data_size = response.getDataLength();
	return *data;
}

/// <summary>
/// Authentications the message1.
/// </summary>
/// <returns>BioAuth.data_size_structure&lt;type&gt;.</returns>
data_size_structure<char*> BioAuth::ClientAuth::authMessage1()
{
	byte dummy[] = { 0x00 };
	Apdu* apdu = new ApduRequest(Configuration::APPLICATION_CLA, Configuration::OFFSET_INS_BIOMETRIC,
		OFFSET_P1_AUTH_FIRST_STEP, 0x00, 0x01, 0x00);
	apdu->wrap(dummy, 1);
	Apdu& response = this->card.sendApdu(*apdu);
#ifdef LOG
	std::cout << response.toString();
#endif

	data_size_structure<char*>* data = new data_size_structure<char*>();
	data->data = (char*)malloc(response.getDataLength());
	memcpy(data->data, response.getData(), response.getDataLength());
	data->data_size = response.getDataLength();

	delete apdu;
	return *data;
}

/// <summary>
/// Authentications the message2.
/// </summary>
/// <param name="input">The input.</param>
/// <returns>data_size_structure&lt;char*&gt;&.</returns>
data_size_structure<char*>& BioAuth::ClientAuth::authMessage2(const data_size_structure<char*>& input)
{
	Apdu* apdu = new ApduRequest(Configuration::APPLICATION_CLA, Configuration::OFFSET_INS_BIOMETRIC,
		OFFSET_P1_AUTH_SECOND_STEP, 0x00, input.data_size, 0x00);
	apdu->wrap((byte*)(input.data), input.data_size);
	Apdu& response = this->card.sendApdu(*apdu);
	std::cout << response.toString();

	data_size_structure<char*>* data = new data_size_structure<char*>();
	data->data = (char*)malloc(response.getDataLength());
	memcpy(data->data, response.getData(), response.getDataLength());
	data->data_size = response.getDataLength();
	delete apdu;
	return *data;

}

#ifdef DEBUG
/// <summary>
/// Gets the M1_ n2.
/// </summary>
void BioAuth::ClientAuth::getM1_N2()
{
	byte dummy[] = { 0x00 };
	Apdu* apdu = new ApduRequest(Configuration::APPLICATION_CLA, Configuration::OFFSET_INS_BIOMETRIC,
		OFFSET_P1_GET_M2_N1, 0x00, 0x01, 0x00);
	apdu->wrap(dummy, 1);
	Apdu& response = this->card.sendApdu(*apdu);
#ifdef LOG
	printf("M1_N2=\n");
	std::cout << response.toString();
#endif
	delete apdu;
}
#endif

/// <summary>
/// Gets the configuration data.
/// </summary>
/// <returns>char *.</returns>
char * BioAuth::ClientAuth::getConfigData()
{
	char* data = (char*)malloc(8);
	short offset = 0;
	memcpy(data + offset, LENGTH, 2);

	offset += 2;
	memcpy(data + offset, M, 2);
	offset += 2;
	memcpy(data + offset, T, 2);
	offset += 2;
	memcpy(data + offset, K, 2);
	return data;
}

/// <summary>
/// Gets the instance.
/// </summary>
/// <returns>BioAuth.ClientAuth *.</returns>
ClientAuth * BioAuth::ClientAuth::getInstance()
{
	if (ClientAuth::pInstance == nullptr)
		pInstance = new ClientAuth();
	return pInstance;
}

/// <summary>
/// Setups the specified card.
/// </summary>
/// <param name="card">The card.</param>
void BioAuth::ClientAuth::setup(Card & card)
{
	this->card = card;
}

/// <summary>
/// Destroys the instance.
/// </summary>
void BioAuth::ClientAuth::destroyInstance()
{
	if (ClientAuth::pInstance != nullptr)
	{
		delete pInstance;
	}
}

/// <summary>
/// Loads the biometrics information.
/// </summary>
/// <param name="matlabFile">The matlab file.</param>
void ClientAuth::loadBiometricsInfo(const char*matlabFile)
{
	FILE * f = fopen(matlabFile, "rb");
	fseek(f, 0, 2);
	int length = ftell(f);
	biometrics.data = (byte*)malloc(SIZE);
	rewind(f);
	fread(biometrics.data, length, 1, f);
	for (short i = 0; i < length; i++)
		this->biometrics.data[i] -= '0';
	biometrics.data_size = length;
	fclose(f);
}