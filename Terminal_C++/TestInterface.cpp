// ***********************************************************************
// Assembly         : 
// Author           : Alberto-PC
// Created          : 06-08-2016
//
// Last Modified By : Alberto-PC
// Last Modified On : 06-08-2016
// ***********************************************************************
// <copyright file="TestInterface.cpp" company="Military Technical Academy">
//     Copyright (c) . All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************
#include "TestInterface.h"

/// <summary>
/// Setups this instance.
/// </summary>
void TestInterface::setup()
{
	byte selectAIDLocal[] = { 0x00,0xA4,0x04,0x00,0xb,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x00,0x00,0x7F };
	selectAID = (byte*)malloc(50);
	memcpy(selectAID, selectAIDLocal, 17);
	selectAIDlen = 17;
	c.connectToSlot();
	Apdu * req = new ApduSelection(selectAID, selectAIDlen);
	Apdu & response = c.sendApdu(*req);
	StatusSW sw = response.getStatus();
	sw.validate();
#ifdef LOG
	std::cout << std::hex << sw.toString();
#endif
	delete req;


}

/// <summary>
/// Tests this instance.
/// </summary>
/// <returns>bool.</returns>
bool TestInterface::Test()
{
	byte ID[] = { 0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17 };
	size_t ID_Size = 0x08;
	byte Password[] = { 0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17 };
	size_t Password_Size = 0x08;
	size_t type,type_auth;
#ifdef LOG
#undef LOG
#endif
#pragma region Register_User
	for (auto it = Features.begin(); it != Features.end(); it++)
	{
		try
		{
			const char* feature = (*it).first.c_str();
			type = (*it).second;

			BioAuth::ClientAuth* client = BioAuth::ClientAuth::getInstance();
			client->setup(c);
			client->setId(ID, ID_Size);
			client->setPassword(Password, Password_Size);
			client->loadBiometricsInfo(feature);
			client->storeData();
			BioAuth::data_size_structure<char*> data = client->registration();
			BioAuth::ServerAuth* server = BioAuth::ServerAuth::getInstance();
			server->setup(c, data);
			server->getRegistrationMessage();
			break;
		}
		catch (std::exception exc)
		{
#ifdef LOG
			std::cout << exc.what() << std::endl;
#endif
			return false; //here the test failed 
		}
	}
		auto iterator_auth = Features.begin()++;
		const char*feature = (*iterator_auth).first.c_str();
		type_auth = (*iterator_auth).second;
		try
		{
			BioAuth::ClientAuth* client = BioAuth::ClientAuth::getInstance();
			client->setup(c);
			client->setId(ID, ID_Size);
			client->setPassword(Password, Password_Size);
			client->loadBiometricsInfo(feature);
			client->storeAuthData();

			BioAuth::data_size_structure<char*> data = client->authMessage1();
			BioAuth::ServerAuth* server = BioAuth::ServerAuth::getInstance();
			server->getS1();
			data = server->authMessage2(&data);
			data = client->authMessage2(data);
			server->authMessage3(&data);
		}
		catch (std::exception exc)
		{
			if (type == type_auth)
				return false;
			else
				return true;
		}
		if (type == type_auth)
			return true;
		else
			return false;
	#pragma endregion
}

/// <summary>
/// Initializes a new instance of the <see cref="TestInterface"/> class.
/// </summary>
TestInterface::TestInterface()
{
	Features.insert(std::make_pair<std::string, int>("feature1.txt", 1));
	Features.insert(std::make_pair<std::string, int>("feature2.txt", 1));
	Features.insert(std::make_pair<std::string, int>("feature3.txt", 2));
	setup();
}

/// <summary>
/// Registers the test.
/// </summary>
void TestInterface::registerTest()
{
	byte selectAID[] = { 0x00,0xA4,0x04,0x00,0xb,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x00,0x00,0x7F };
	short selectAIDlen = 17;
	Card c;
	c.connectToSlot();

	Apdu * req = new ApduSelection(selectAID, selectAIDlen);
	Apdu & response = c.sendApdu(*req);
	StatusSW sw = response.getStatus();
	std::cout << std::hex << sw.toString();

	byte ID[] = { 0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17 };
	size_t ID_Size = 0x08;
	byte Password[] = { 0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17 };
	size_t Password_Size = 0x08;


	BioAuth::ClientAuth* client = BioAuth::ClientAuth::getInstance();
	client->setup(c);
	client->setId(ID, ID_Size);
	client->setPassword(Password, Password_Size);
	client->loadBiometricsInfo("feature1.txt");
	client->storeData();

	BioAuth::data_size_structure<char*> data = client->registration();

	BioAuth::ServerAuth* server = BioAuth::ServerAuth::getInstance();
	server->setup(c, data);
	server->getRegistrationMessage();

	// for test
	server->getP();
	server->getQ();
	server->getS3();


	delete req;

}

/// <summary>
/// Authentications the test.
/// </summary>
void TestInterface::authTest()
{
	byte selectAID[] = { 0x00,0xA4,0x04,0x00,0xb,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x00,0x00,0x7F };
	short selectAIDlen = 17;
	Card c;
	c.connectToSlot();

	Apdu * req = new ApduSelection(selectAID, selectAIDlen);
	Apdu & response = c.sendApdu(*req);
	StatusSW sw = response.getStatus();
	std::cout << std::hex << sw.toString();

	byte ID[] = { 0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17 };
	size_t ID_Size = 0x08;
	byte Password[] = { 0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17 };
	size_t Password_Size = 0x08;


	BioAuth::ClientAuth* client = BioAuth::ClientAuth::getInstance();
	client->setup(c);
	client->setId(ID, ID_Size);
	client->setPassword(Password, Password_Size);
	client->loadBiometricsInfo("feature2.txt");
	client->storeAuthData();

	BioAuth::data_size_structure<char*> data = client->authMessage1();
	BioAuth::ServerAuth* server = BioAuth::ServerAuth::getInstance();
	server->getS1();

	data = server->authMessage2(&data);
	data = client->authMessage2(data);
	server->authMessage3(&data);

	//test the sk
	client->getSk();
	client->getM1_N2();

	delete req;
}

/// <summary>
/// Finalizes an instance of the <see cref="TestInterface"/> class.
/// </summary>
TestInterface::~TestInterface()
{
	free(selectAID);
}
