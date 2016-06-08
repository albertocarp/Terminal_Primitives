// ***********************************************************************
// Assembly         : 
// Author           : Alberto-PC
// Created          : 05-12-2016
//
// Last Modified By : Alberto-PC
// Last Modified On : 06-02-2016
// ***********************************************************************
// <copyright file="Apdu.h" company="Military Technical Academy">
//     Copyright (c) . All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************
#pragma once
#include "defs.h"
#include "StatusSW.h"
#include "CardSession.h"
/// <summary>
/// Class Apdu.
/// </summary>
class Apdu
{
public:
	/// <summary>
	/// The is o7816_ offse t_ cla
	/// </summary>
	static final ISO7816_OFFSET_CLA = 0;
	/// <summary>
	/// The is o7816_ offse t_ ins
	/// </summary>
	static final ISO7816_OFFSET_INS = 1;
	/// <summary>
	/// The is o7816_ offse t_ p1
	/// </summary>
	static final ISO7816_OFFSET_P1 = 2;
	/// <summary>
	/// The is o7816_ offse t_ p2
	/// </summary>
	static final ISO7816_OFFSET_P2 = 3;
	/// <summary>
	/// The is o7816_ offse t_ lc
	/// </summary>
	static final ISO7816_OFFSET_LC = 4;
	/// <summary>
	/// The is o7816_ offse t_ cdata
	/// </summary>
	static final ISO7816_OFFSET_CDATA = 5;
protected:
	/// <summary>
	/// The raw data
	/// </summary>
	byte * rawData;
	/// <summary>
	/// The data length
	/// </summary>
	word dataLength;
	/// <summary>
	/// The time_elpsed
	/// </summary>
	double time_elpsed;
public:
	/// <summary>
	/// Gets the data.
	/// </summary>
	/// <returns>const byte *.</returns>
	const	byte*	 getData()const;
	/// <summary>
	/// Gets the non constant data.
	/// </summary>
	/// <returns>byte *.</returns>
	byte*			 getNonConstantData();
	/// <summary>
	/// Gets the length of the data.
	/// </summary>
	/// <returns>const word.</returns>
	virtual const	word	 getDataLength()const;
	/// <summary>
	/// Gets the length of the apdu.
	/// </summary>
	/// <returns>const word.</returns>
	virtual const	word	 getApduLength()const;
	/// <summary>
	/// Gets the expected length.
	/// </summary>
	/// <returns>byte.</returns>
	virtual byte	 getExpectedLen()const = 0;
	/// <summary>
	/// Determines whether this instance has status.
	/// </summary>
	/// <returns>bool.</returns>
	virtual bool	 hasStatus() = 0;
	/// <summary>
	/// Gets the status.
	/// </summary>
	/// <returns>StatusSW.</returns>
	virtual StatusSW getStatus() = 0;
	/// <summary>
	/// Wraps the specified p data.
	/// </summary>
	/// <param name="pData">The p data.</param>
	/// <param name="pLength">Length of the p.</param>
	virtual void wrap(byte* pData, word pLength) = 0;
	/// <summary>
	/// Wraps the specified data.
	/// </summary>
	/// <param name="data">The data.</param>
	virtual void wrap(vArray* data) = 0;
	/// <summary>
	/// Updates the specified data.
	/// </summary>
	/// <param name="data">The data.</param>
	virtual void update(Apdu* data);
	/// <summary>
	/// Determines whether this instance has data.
	/// </summary>
	/// <returns>bool.</returns>
	bool hasData();
	/// <summary>
	/// To the string.
	/// </summary>
	/// <returns>std.string.</returns>
	virtual std::string toString()const = 0;
	/// <summary>
	/// Gets the time elapsed.
	/// </summary>
	/// <returns>double.</returns>
	double getTimeElapsed();
	/// <summary>
	/// Initializes a new instance of the <see cref="Apdu"/> class.
	/// </summary>
	Apdu();
	/// <summary>
	/// Finalizes an instance of the <see cref="Apdu"/> class.
	/// </summary>
	~Apdu();
};

