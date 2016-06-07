#pragma once
#include "defs.h"
#include "Card.h"
#include"Convert.h"
#include "GaloisField.h"
namespace BioAuth
{
	template<typename type>
	struct data_size_structure
	{
		type  data;
		size_t data_size;
	};

	class ClientAuth : Config
	{
	private:
		data_size_structure<byte*>  id;
		data_size_structure<byte*>  password;
		data_size_structure<byte*>  biometrics;
		static ClientAuth* pInstance;
		Card card;
	public :
		static const int MAX_BIOMETRICS_POINTS = 23;
		static const int ENCODED_POINT_LENGTH  = 20;
		static const int SIZE = MAX_BIOMETRICS_POINTS * ENCODED_POINT_LENGTH;
	public:
		ClientAuth(const Card& card);
		void setId(byte* id, size_t size);
		void setPassword(byte* password, size_t size);
		void setBiometricsInfo(byte* biometrics, size_t size);
		void loadBiometricsInfo(const char*matlabFile);
		void storeData();
		void storeAuthData();
		data_size_structure<char*> registration();
		data_size_structure<char*> authMessage1();
		data_size_structure<char*>& authMessage2(const data_size_structure<char*>& input);

		char* getConfigData();
		static ClientAuth* getInstance();
		void setup(Card& card);

		// For testing mode
		char* getPassword();
		void  getData();
		void  getGalois();
		void  getBiometrics();
		void getSk();
		~ClientAuth();
	private:
		ClientAuth();
	};
}

