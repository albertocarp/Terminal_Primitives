#pragma once
#include"defs.h"
#include <BigNumber.h>
#include <Hash.h>
#include "Helper.h"
#include "Card.h"
#include "ClientAuth.h"
namespace BioAuth
{

	class ServerAuth : Config
	{
	
	private:
		char* ID;
		size_t size_ID;
		char* hash_R;
		char* hash_R_PW;
		char* S3;
		char* S1;
		char* M1;
		char* M2;
		BigNumber p;
		BigNumber q;
		BigNumber x;
		BigNumber N2;
		size_t x_size;
		Card card;

	private:
		static ServerAuth* p_Instance;
		template<typename t>
		ServerAuth(Card& card, data_size_structure<t>data);
		ServerAuth();
	public:
		void getRegistrationMessage();

		void setXSize(size_t x);
		void getP();
		void getQ();
		void getS3();
		void getS1();

		static ServerAuth* getInstance();
		template<typename t>
		void setup(Card & card, data_size_structure<t> data);
		template<typename t>
		data_size_structure<t> authMessage2(data_size_structure<t>* data);
		template<typename t>
		void authMessage3(data_size_structure<t>* data);
		~ServerAuth();

	private:
		template<typename t>
		void fetch_data(data_size_structure<t>data);
	};
	template<typename t>
	inline ServerAuth::ServerAuth(Card & card,data_size_structure<t> data)
	{
		this->card = card;
		fetch_data(data);
		S3 = (char*)malloc(HASH_SIZE);
	}
	template<typename t>
	inline void ServerAuth::setup(Card & card,data_size_structure<t> data)
	{
		this->card = card;
		fetch_data(data);
	}
	template<typename t>
	inline data_size_structure<t> ServerAuth::authMessage2(data_size_structure<t>* data)
	{
		char* data2 = data->data;
		short offset = 0;
		unsigned char* bn_string = nullptr;
		M1 = (char*)malloc(128);
		M2 = (char*)malloc(128);
		int val;
		BigNumber bn(S1);
#ifdef LOG
		printf("Id_recv \n");
		Helper::print_hexa(data2, size_ID);

		printf("a recv \n");
		Helper::print_hexa(data2 + size_ID, HASH_SIZE);

		printf("message received \n");
		Helper::print_hexa(data2 + size_ID + HASH_SIZE, 128);
#endif // LOG
		memcpy(M1, data2 + size_ID + HASH_SIZE, 128);
		size_t data_size = data->data_size;
		short message_size = 128;
		char * a_prim = (char*)malloc(128 + size_ID + HASH_SIZE+10);

		memcpy(a_prim, ID, size_ID);
		memcpy(a_prim + size_ID, data2 + data_size- message_size, 128);
		memcpy(a_prim+size_ID+ message_size,S1,HASH_SIZE);
#ifdef LOG
		printf("message constructed \n");
		Helper::print_hexa((char*)a_prim, data_size);
#endif // LOG
		char* hash = (char*)Hash::sha1_v1((unsigned char*)a_prim,data_size, 0);
#ifdef LOG
	printf("hash(R)\n");
	Helper::print_hexa((char*)hash, HASH_SIZE);
#endif // LOG
		val = memcmp(data2 + size_ID, hash, HASH_SIZE);
		if (val != 0)
		{
			throw new exception("User authentification failed");
		}
		N2.Generate_prime_BigNumber(HASH_SIZE*8);
		N2.mod(this->q);
		bn.modExp(N2, p);
		bn_string = bn.ToString();
		memset(a_prim, 0x00, 128 + size_ID + HASH_SIZE);
		memcpy(a_prim+offset, ID, size_ID);
		offset += size_ID;
		memcpy(a_prim+offset, S1, HASH_SIZE);
		offset += HASH_SIZE;
		memcpy(a_prim+offset, bn_string, 128);
		offset += 128;

		free(hash);
		hash = (char*)Hash::sha1_v1((unsigned char*)a_prim, offset, 0);

#ifdef LOG
		printf("(ID || S1 || M2)\n");
		Helper::print_hexa((char*)a_prim, offset);

		printf("h(ID || S1 || M2)\n");
		Helper::print_hexa((char*)hash, HASH_SIZE);
#endif // LOG

		data_size_structure<char*>* structure = new data_size_structure<char*>();
		structure->data_size = 128 + HASH_SIZE;
		structure->data = (char*)malloc(structure->data_size);
		memcpy(structure->data, hash, HASH_SIZE);
		memcpy(structure->data + HASH_SIZE, bn_string, SIZE_P);
		memcpy(M2, bn_string, 128);
#ifdef LOG
		printf("{b M2}\n");
		Helper::print_hexa(structure->data, structure->data_size);
#endif // LOG
		free(hash);
		free(a_prim);
		return *structure;
	}
	template<typename t>
	inline void  ServerAuth::authMessage3(data_size_structure<t>* data)
	{
		size_t data_size = data->data_size;
		char* data2 = data->data;
		short offset = 0;
		char * c_prim = (char*)malloc(128 + size_ID + HASH_SIZE + 128);
		memcpy(c_prim +offset,M1, SIZE_MESSAGES);
		offset += 128;
		memcpy(c_prim + offset, ID, size_ID);
		offset += size_ID;
		memcpy(c_prim + offset, S1, HASH_SIZE);
		offset += HASH_SIZE;
		memcpy(c_prim + offset, M2, SIZE_MESSAGES);
		offset += SIZE_MESSAGES;
		char*hash = (char*)Hash::sha1_v1((unsigned char*)c_prim, offset, 0);
#ifdef LOG
		printf("h{M1||ID||S1||M2}\n");
		Helper::print_hexa(hash, HASH_SIZE);
#endif // LOG
		int val = memcmp(hash, data2, HASH_SIZE);
		if (val != 0)
		{
			throw new exception("User authentification failed");
		}

	}
	template<typename t>
	inline void ServerAuth::fetch_data(data_size_structure<t> data)
	{
		size_t data_size = data.data_size;

		if(data_size <  2 * HASH_SIZE)
			throw new exception("Invalid data length");
		size_ID = data_size - 2 * HASH_SIZE;
		this->ID = (char*)malloc(this->size_ID);
		memcpy(this->ID, data.data, size_ID);
		hash_R = (char*)malloc(HASH_SIZE);
		memcpy(this->hash_R, data.data + size_ID, HASH_SIZE);

#ifdef LOG
		printf("Hash r=\n");
		Helper::print_hexa(hash_R, HASH_SIZE);
#endif // LOG
		// copy S1 to final message;
		hash_R_PW = (char*)malloc(HASH_SIZE);
		memcpy(this->hash_R_PW, data.data + size_ID+HASH_SIZE, HASH_SIZE);
#ifdef LOG
		printf("Hash_r_pw=\n");
		Helper::print_hexa(hash_R_PW, HASH_SIZE);
#endif // LOG
		setXSize(40);
	}
}
