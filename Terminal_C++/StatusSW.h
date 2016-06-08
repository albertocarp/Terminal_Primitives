// ***********************************************************************
// Assembly         : 
// Author           : Alberto-PC
// Created          : 05-12-2016
//
// Last Modified By : Alberto-PC
// Last Modified On : 05-17-2016
// ***********************************************************************
// <copyright file="StatusSW.h" company="">
//     Copyright (c) . All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************
#pragma once
#include "defs.h"
#include "SmartCardException.h"
/// <summary>
/// Class StatusSW.
/// </summary>
class StatusSW
{
private:
	/// <summary>
	/// The SW1
	/// </summary>
	byte sw1;
	/// <summary>
	/// The SW2
	/// </summary>
	byte sw2;
public:

	/// <summary>
	/// To the string.
	/// </summary>
	/// <returns>const std.string.</returns>
	const std::string toString();

	/// <summary>
	/// Initializes a new instance of the <see cref="StatusSW" /> class.
	/// </summary>
	/// <param name="sw1">The SW1.</param>
	/// <param name="sw2">The SW2.</param>
	StatusSW(byte sw1, byte sw2);

	/// <summary>
	/// ses the w1.
	/// </summary>
	/// <returns>byte.</returns>
	byte SW1();

	/// <summary>
	/// ses the w2.
	/// </summary>
	/// <returns>byte.</returns>
	byte SW2();

	/// <summary>
	/// Validates this instance.
	/// </summary>
	void validate();

	/// <summary>
	/// Operator==s the specified RHS.
	/// </summary>
	/// <param name="rhs">The RHS.</param>
	/// <returns>bool.</returns>
	bool operator==(const StatusSW & rhs);

	/// <summary>
	/// Finalizes an instance of the <see cref="StatusSW" /> class.
	/// </summary>
	~StatusSW();
};

