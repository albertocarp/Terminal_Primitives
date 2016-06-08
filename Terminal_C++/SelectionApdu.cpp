// ***********************************************************************
// Assembly         : 
// Author           : Alberto-PC
// Created          : 05-12-2016
//
// Last Modified By : Alberto-PC
// Last Modified On : 05-14-2016
// ***********************************************************************
// <copyright file="SelectionApdu.cpp" company="">
//     Copyright (c) . All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************
#include "SelectionApdu.h"


/// <summary>
/// Initializes a new instance of the <see cref="ApduSelection"/> class.
/// </summary>
/// <param name="selectAID">The select aid.</param>
/// <param name="selectLength">Length of the select.</param>
ApduSelection::ApduSelection(byte * selectAID, word selectLength)
{
	rawData = new byte[selectLength + 1];
	memcpy(rawData, selectAID, selectLength);
	this->dataLength = selectLength;

}

/// <summary>
/// Finalizes an instance of the <see cref="ApduSelection"/> class.
/// </summary>
ApduSelection::~ApduSelection()
{
}

/// <summary>
/// Determines whether this instance has status.
/// </summary>
/// <returns>bool.</returns>
bool ApduSelection::hasStatus()
{
	return true;
}

/// <summary>
/// Gets the status.
/// </summary>
/// <returns>StatusSW.</returns>
StatusSW ApduSelection::getStatus()
{
	return *this->status;
}

/// <summary>
/// Gets the expected length.
/// </summary>
/// <returns>byte.</returns>
byte ApduSelection::getExpectedLen()const
{
	return (byte)258;
}

/// <summary>
/// Wraps the specified p data.
/// </summary>
/// <param name="pData">The p data.</param>
/// <param name="pLength">Length of the p.</param>
void ApduSelection::wrap(byte * pData, word pLength)
{
	//TODO fix this
}

/// <summary>
/// Wraps the specified data.
/// </summary>
/// <param name="data">The data.</param>
void ApduSelection::wrap(vArray * data)
{
}

/// <summary>
/// To the string.
/// </summary>
/// <returns>std.string.</returns>
std::string ApduSelection::toString() const
{
	return std::string();
}
