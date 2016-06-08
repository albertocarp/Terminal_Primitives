// ***********************************************************************
// Assembly         : 
// Author           : Alberto-PC
// Created          : 05-30-2016
//
// Last Modified By : Alberto-PC
// Last Modified On : 06-08-2016
// ***********************************************************************
// <copyright file="ServerAuth.h" company="Military Technical Academy">
//     Copyright (c) . All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************
#pragma once

#include"defs.h"
#include <BigNumber.h>
#include <Hash.h>
#include "Helper.h"
#include "Card.h"
#include "ClientAuth.h"
/// <summary>
/// The BioAuth namespace.
/// </summary>
namespace BioAuth
{

	/// <summary>
	/// Class ServerAuth.
	/// </summary>
	/// <seealso cref="Config" />
	class ServerAuth : Config
	{
	
	private:
		/// <summary>
		/// The identifier
		/// </summary>
		char* ID;
		/// <summary>
		/// The size_ identifier
		/// </summary>
		size_t size_ID;
		/// <summary>
		/// The hash_ r
		/// </summary>
		char* hash_R;
		/// <summary>
		/// The hash_ r_ pw
		/// </summary>
		char* hash_R_PW;
		/// <summary>
		/// The s3
		/// </summary>
		char* S3;
		/// <summary>
		/// The s1
		/// </summary>
		char* S1;
		/// <summary>
		/// The m1
		/// </summary>
		char* M1;
		/// <summary>
		/// The m2
		/// </summary>
		char* M2;
		/// <summary>
		/// The sk
		/// </summary>
		char *Sk;
		/// <summary>
		/// The p
		/// </summary>
		BigNumber p;
		/// <summary>
		/// The q
		/// </summary>
		BigNumber q;
		/// <summary>
		/// The x
		/// </summary>
		BigNumber x;
		/// <summary>
		/// The n2
		/// </summary>
		BigNumber N2;
		/// <summary>
		/// The x_size
		/// </summary>
		size_t x_size;
		/// <summary>
		/// The card
		/// </summary>
		Card card;

	private:
		static ServerAuth* p_Instance;

		/// <summary>
		/// Initializes a new instance of the <see cref="ServerAuth" /> class.
		/// </summary>
		/// <param name="card">The card.</param>
		/// <param name="data">The data.</param>
		/// /
		/// <summary>
		/// Initializes a new instance of the <see cref="ServerAuth"/> class.
		/// </summary>
		/// <param name="card">The card.</param>
		/// <param name="data">The data.</param>
		/// <summary>
		/// Initializes a new instance of the <see cref="ServerAuth"/> class.
		/// </summary>
		/// <param name="card">The card.</param>
		/// <param name="data">The data.</param>
		template<typename t>
		ServerAuth(Card& card, data_size_structure<t>data);
		/// <summary>
		/// Prevents a default instance of the <see cref="ServerAuth" /> class from being created.
		/// </summary>
		ServerAuth();
		/// <summary>
		/// Computes the sk.
		/// </summary>
		void computeSk();
	public:
		/// <summary>
		/// Gets the registration message.
		/// </summary>
		void getRegistrationMessage();

		/// <summary>
		/// Sets the size of the x.
		/// </summary>
		/// <param name="x">The x.</param>
		void setXSize(size_t x);
#ifdef DEBUG
		/// <summary>
		/// Gets the p.
		/// </summary>
		void getP();
		/// <summary>
		/// Gets the q.
		/// </summary>
		void getQ();
		/// <summary>
		/// Gets the s3.
		/// </summary>
		void getS3();
		/// <summary>
		/// Gets the s1.
		/// </summary>
		void getS1();
#endif // DEBUG

		/// <summary>
		/// Gets the instance.
		/// </summary>
		/// <returns>BioAuth.ServerAuth *.</returns>
		static ServerAuth* getInstance();

		/// <summary>
		/// Setups the specified card.
		/// </summary>
		/// <param name="card">The card.</param>
		/// <param name="data">The data.</param>
		template<typename t>
		void setup(Card & card, data_size_structure<t> data);
		/// <summary>
		/// Authentications the message2.
		/// </summary>
		/// <param name="data">The data.</param>
		/// <returns>BioAuth.data_size_structure&lt;type&gt;.</returns>
		template<typename t>
		data_size_structure<t> authMessage2(data_size_structure<t>* data);
		/// <summary>
		/// Authentications the message3.
		/// </summary>
		/// <param name="data">The data.</param>
		template<typename t>
		void authMessage3(data_size_structure<t>* data);
		/// <summary>
		/// Destroys the instance.
		/// </summary>
		static void destroyInstance();
	private:
		/// <summary>
		/// Fetch_datas the specified data.
		/// </summary>
		/// <param name="data">The data.</param>
		template<typename t>
		void fetch_data(data_size_structure<t>data);
		/// <summary>
		/// Finalizes an instance of the <see cref="ServerAuth" /> class.
		/// </summary>
		~ServerAuth();
	};




	/// <summary>
	/// Initializes a new instance of the <see cref="ServerAuth" /> class.
	/// </summary>
	/// <param name="card">The card.</param>
	/// <param name="data">The data.</param>
	/// /
	template<typename t>
	inline ServerAuth::ServerAuth(Card & card,data_size_structure<t> data)
	{
		this->card = card;
		fetch_data(data);
		S3 = (char*)malloc(HASH_SIZE);
	}

	/// <summary>
	/// Setups the specified card.
	/// </summary>
	/// <param name="card">The card.</param>
	/// <param name="data">The data.</param>
	template<typename t>
	inline void ServerAuth::setup(Card & card,data_size_structure<t> data)
	{
		this->card = card;
		fetch_data(data);
	}


	/// <summary>
	/// Authentications the message2.
	/// </summary>
	/// <param name="data">The data.</param>
	/// <returns>BioAuth.data_size_structure&lt;type&gt;.</returns>
	template<typename t>
	inline data_size_structure<t> ServerAuth::authMessage2(data_size_structure<t>* data)
	{
		char* data2 = data->data;
		size_t offset = 0;
		unsigned char* bn_string = nullptr;
		M1 = (char*)malloc(128);
		M2 = (char*)malloc(128);
		int val;
		BigNumber bn((uch*)S1, HASH_SIZE);
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
		unsigned char * a_prim = (unsigned char*)malloc(128 + size_ID + HASH_SIZE+10);

		memcpy(a_prim, ID, size_ID);
		memcpy(a_prim + size_ID, data2 + data_size- message_size, 128);
		memcpy(a_prim+size_ID+ message_size,S1,HASH_SIZE);
#ifdef LOG
		printf("message constructed \n");
		Helper::print_hexa((char*)a_prim, data_size);
#endif // LOG
		char* hash = (char*)Hash::sha1_v1(a_prim,data_size, 0);
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
		//N2.mod(this->q);
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


	/// <summary>
	/// Authentications the message3.
	/// </summary>
	/// <param name="data">The data.</param>
	template<typename t>
	inline void  ServerAuth::authMessage3(data_size_structure<t>* data)
	{
		size_t data_size = data->data_size;
		char* data2 = data->data;
		size_t offset = 0;
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
		printf("{M1||ID||S1||M2}\n");
		Helper::print_hexa(c_prim, offset);

		printf("h{M1||ID||S1||M2}\n");
		Helper::print_hexa(hash, HASH_SIZE);
#endif // LOG
		int val = memcmp(hash, data2, HASH_SIZE);
		if (val != 0)
		{
			throw new exception("User authentification failed");
		}
		computeSk();

	}


	/// <summary>
	/// Fetch_datas the specified data.
	/// </summary>
	/// <param name="data">The data.</param>
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
