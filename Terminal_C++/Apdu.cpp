// ***********************************************************************
// Assembly         : 
// Author           : Alberto-PC
// Created          : 05-12-2016
//
// Last Modified By : Alberto-PC
// Last Modified On : 06-08-2016
// ***********************************************************************
// <copyright file="Apdu.cpp" company="Military Technical Academy">
//     Copyright (c) . All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************
#include "Apdu.h"



/// <summary>
/// Gets the data.
/// </summary>
/// <returns>const byte *.</returns>
const byte * Apdu::getData()const
{
	return rawData;
}

/// <summary>
/// Gets the non constant data.
/// </summary>
/// <returns>byte *.</returns>
byte * Apdu::getNonConstantData()
{
	return rawData;
}

/// <summary>
/// Gets the length of the data.
/// </summary>
/// <returns>const word.</returns>
const word Apdu::getDataLength()const
{
	return dataLength;
}

/// <summary>
/// Gets the length of the apdu.
/// </summary>
/// <returns>const word.</returns>
const word Apdu::getApduLength() const
{
	return dataLength;
}

/// <summary>
/// Updates the specified data.
/// </summary>
/// <param name="data">The data.</param>
void Apdu::update(Apdu * data)
{
}

/// <summary>
/// Determines whether this instance has data.
/// </summary>
/// <returns>bool.</returns>
bool Apdu::hasData()
{
	if (dataLength > 0)
		return true;
	return false;
}

/// <summary>
/// Gets the time elapsed.
/// </summary>
/// <returns>double.</returns>
double Apdu::getTimeElapsed()
{
	return time_elpsed;
}

/// <summary>
/// Initializes a new instance of the <see cref="Apdu"/> class.
/// </summary>
Apdu::Apdu()
{
}


/// <summary>
/// Finalizes an instance of the <see cref="Apdu"/> class.
/// </summary>
Apdu::~Apdu()
{
	delete this->rawData;
}
