// ***********************************************************************
// Assembly         : 
// Author           : Alberto-PC
// Created          : 06-08-2016
//
// Last Modified By : Alberto-PC
// Last Modified On : 06-08-2016
// ***********************************************************************
// <copyright file="TestInterface.h" company=Military Techincal Academy"">
//     Copyright (c) . All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************
#pragma once
#include "ServerAuth.h"
#include "ClientAuth.h"

/// <summary>
/// Class TestInterface.
/// </summary>
class TestInterface
{
	/// <summary>
	/// The features
	/// </summary>
	std::map<std::string, int> Features;
	/// <summary>
	/// The select aid
	/// </summary>
	byte* selectAID;
	/// <summary>
	/// The c
	/// </summary>
	Card c;
	/// <summary>
	/// The select ID length
	/// </summary>
	size_t selectAIDlen;
private:
	/// <summary>
	/// Setups this instance.
	/// </summary>
	void setup();
public:
	/// <summary>
	/// Tests this instance.
	/// </summary>
	/// <returns>bool.</returns>
	bool Test();
	/// <summary>
	/// Initializes a new instance of the <see cref="TestInterface"/> class.
	/// </summary>
	TestInterface();
	/// <summary>
	/// Finalizes an instance of the <see cref="TestInterface"/> class.
	/// </summary>
	~TestInterface();
	/// <summary>
	/// Registers the test.
	/// </summary>
	void registerTest();
	/// <summary>
	/// Authentications the test.
	/// </summary>
	void authTest();
};

