// ***********************************************************************
// Assembly         : 
// Author           : Alberto-PC
// Created          : 05-12-2016
//
// Last Modified By : Alberto-PC
// Last Modified On : 05-17-2016
// ***********************************************************************
// <copyright file="StatusSW.cpp" company="">
//     Copyright (c) . All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************
#include "StatusSW.h"



/// <summary>
/// To the string.
/// </summary>
/// <returns>const std.string.</returns>
const std::string StatusSW::toString()
{
	std::stringstream ss;
	ss << "SW1 : ";
	ss << "0x" << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(this->sw1);
	ss << " SW2 : ";
	ss << "0x" << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(this->sw2);
	return ss.str();
}

/// <summary>
/// Initializes a new instance of the <see cref="StatusSW"/> class.
/// </summary>
/// <param name="sw1">The SW1.</param>
/// <param name="sw2">The SW2.</param>
StatusSW::StatusSW(byte sw1, byte sw2)
{
	this->sw1 = sw1;
	this->sw2 = sw2;
}

/// <summary>
/// ses the w1.
/// </summary>
/// <returns>byte.</returns>
byte StatusSW::SW1()
{
	return sw1;
}

/// <summary>
/// ses the w2.
/// </summary>
/// <returns>byte.</returns>
byte StatusSW::SW2()
{
	return sw2;
}

/// <summary>
/// Validates this instance.
/// </summary>
void StatusSW::validate()
{
	if (this->sw1 != 0x90 && this->sw2 != 0x00)
	{
		throw new SmartCardException("Invalid status card");
	}
}

/// <summary>
/// Operator==s the specified RHS.
/// </summary>
/// <param name="rhs">The RHS.</param>
/// <returns>bool.</returns>
bool StatusSW::operator==(const StatusSW & rhs)
{
	if (this->sw1 == rhs.sw1 && this->sw2 == rhs.sw2)
		return true;
	return false;
}


/// <summary>
/// Finalizes an instance of the <see cref="StatusSW"/> class.
/// </summary>
StatusSW::~StatusSW()
{
}
