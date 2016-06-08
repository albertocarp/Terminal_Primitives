// ***********************************************************************
// Assembly         : 
// Author           : Alberto-PC
// Created          : 05-30-2016
//
// Last Modified By : Alberto-PC
// Last Modified On : 06-08-2016
// ***********************************************************************
// <copyright file="ClientAuth.h" company="Military Technical Academy">
//     Copyright (c) . All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************

#pragma once
#include "defs.h"
#include "Card.h"
#include "Convert.h"
#include "GaloisField.h"

/// <summary>
/// The BioAuth namespace.
/// </summary>
namespace BioAuth
{
	
	/// <summary>
	/// Struct data_size_structure
	/// </summary>
	template<typename type>

	struct data_size_structure
	{
		/// <summary>
		/// The data
		/// </summary>
		type  data;
		/// <summary>
		/// The data_size
		/// </summary>
		size_t data_size;
	};
	
	
	/// <summary>
	/// Class ClientAuth.This class handles the interaction
	/// between the off-card client and the on-card applet
	/// </summary>
	/// <seealso cref="Config" />
	class ClientAuth : Config
	{
	private:

		/// <summary>
		/// The id of the user
		/// </summary>
		data_size_structure<byte*>  id;
		/// <summary>
		/// The password
		/// </summary>
		data_size_structure<byte*>  password;
		/// <summary>
		/// The biometrics data
		/// </summary>
		data_size_structure<byte*>  biometrics;
		///
		/// The instance of the current class
		///
		static ClientAuth* pInstance;
		/// <summary>
		/// The card
		/// </summary>
		Card card;
	public :
		/// <summary>
		/// The ma x_ biometric s_ points
		/// </summary>
		static const int MAX_BIOMETRICS_POINTS = 23;
		/// <summary>
		/// The encode d_ poin t_ length
		/// </summary>
		static const int ENCODED_POINT_LENGTH = 20;
		/// <summary>
		/// The size
		/// </summary>
		static const int SIZE = MAX_BIOMETRICS_POINTS * ENCODED_POINT_LENGTH;
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="ClientAuth" /> class.
		/// </summary>
		/// <param name="card">The card.</param>
		ClientAuth(const Card& card);
		/// <summary>
		/// Sets the identifier.
		/// </summary>
		/// <param name="id">The identifier.</param>
		/// <param name="size">The size.</param>
		void setId(byte* id, size_t size);
		/// <summary>
		/// Sets the password.
		/// </summary>
		/// <param name="password">The password.</param>
		/// <param name="size">The size.</param>
		void setPassword(byte* password, size_t size);
		/// <summary>
		/// Sets the biometrics information.
		/// </summary>
		/// <param name="biometrics">The biometrics.</param>
		/// <param name="size">The size.</param>
		void setBiometricsInfo(byte* biometrics, size_t size);
		/// <summary>
		/// Loads the biometrics information.
		/// </summary>
		/// <param name="matlabFile">The matlab file.</param>
		void loadBiometricsInfo(const char*matlabFile);
		/// <summary>
		/// Stores the data.
		/// </summary>
		void storeData();
		/// <summary>
		/// Stores the authentication data.
		/// </summary>
		void storeAuthData();
		/// <summary>
		/// Registrations this instance.
		/// </summary>
		/// <returns>BioAuth.data_size_structure&lt;type&gt;.</returns>
		data_size_structure<char*> registration();
		/// <summary>
		/// Authentications the message1.
		/// </summary>
		/// <returns>BioAuth.data_size_structure&lt;type&gt;.</returns>
		data_size_structure<char*> authMessage1();
		/// <summary>
		/// Authentications the message2.
		/// </summary>
		/// <param name="input">The input.</param>
		/// <returns>data_size_structure&lt;char*&gt;&.</returns>
		data_size_structure<char*>& authMessage2(const data_size_structure<char*>& input);
		/// <summary>
		/// Gets the configuration data.
		/// </summary>
		/// <returns>char *.</returns>
		char* getConfigData();
		/// <summary>
		/// Gets the instance.
		/// </summary>
		/// <returns>BioAuth.ClientAuth *.</returns>
		static ClientAuth* getInstance();
		/// <summary>
		/// Setups the specified card.
		/// </summary>
		/// <param name="card">The card.</param>
		void setup(Card& card);
		/// <summary>
		/// Destroys the instance.
		/// </summary>
		void destroyInstance();
		// For testing mode
#ifdef DEBUG
		/// <summary>
		/// Gets the password.
		/// </summary>
		/// <returns>char *.</returns>
		char* getPassword();
		/// <summary>
		/// Gets the data.
		/// </summary>
		void  getData();
		/// <summary>
		/// Gets the galois.
		/// </summary>
		void  getGalois();
		/// <summary>
		/// Gets the biometrics.
		/// </summary>
		void  getBiometrics();
		/// <summary>
		/// Gets the sk.
		/// </summary>
		void  getSk();
		/// <summary>
		/// Gets the s1.
		/// </summary>
		void  getS1();
		/// <summary>
		/// Gets the M1_ n2.
		/// </summary>
		void  getM1_N2();
#endif
	private:
		/// <summary>
		/// Finalizes an instance of the <see cref="ClientAuth" /> class.
		/// </summary>
		~ClientAuth();
		/// <summary>
		/// Prevents a default instance of the <see cref="ClientAuth" /> class from being created.
		/// </summary>
		ClientAuth();
	};
}

