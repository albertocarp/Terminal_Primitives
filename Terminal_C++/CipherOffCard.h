// ***********************************************************************
// Assembly         : 
// Author           : Alberto-PC
// Created          : 05-14-2016
//
// Last Modified By : Alberto-PC
// Last Modified On : 05-23-2016
// ***********************************************************************
// <copyright file="CipherOffCard.h" company="">
//     Copyright (c) . All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************
#pragma once
#include "defs.h"
#include "Card.h"
/// <summary>
/// Class CipherOffCard.
/// </summary>
class CipherOffCard
{
private:
	/// <summary>
	/// The myseed
	/// </summary>
	int myseed = 1234;
	/// <summary>
	/// Initializes a new instance of the <see cref="CipherOffCard"/> class.
	/// </summary>
	/// <param name="CIPHER">The cipher.</param>
	CipherOffCard(final CIPHER);
	static byte CURRENT_CIPHER;
	/// <summary>
	/// Finalizes an instance of the <see cref="CipherOffCard"/> class.
	/// </summary>
	~CipherOffCard();
public:

	/// <summary>
	/// The mod e_ encrypt
	/// </summary>
	static final MODE_ENCRYPT = 0x21;
	/// <summary>
	/// The mod e_ decrypt
	/// </summary>
	static final MODE_DECRYPT = 0x22;
	/// <summary>
	/// The mod e_ generat e_ key
	/// </summary>
	static final MODE_GENERATE_KEY = 0x23;
	
	/// <summary>
	/// The ciphe r_ picollo
	/// </summary>
	static final CIPHER_PICOLLO = 0x34;
	/// <summary>
	/// The ciphe r_ twine
	/// </summary>
	static final CIPHER_TWINE = 0x30;
	/// <summary>
	/// The ciphe r_ lblock
	/// </summary>
	static final CIPHER_LBLOCK = 0x32;
	/// <summary>
	/// The ciphe r_ rectangle
	/// </summary>
	static final CIPHER_RECTANGLE = 0x35;
	/// <summary>
	/// The ciphe r_ zorro
	/// </summary>
	static final CIPHER_ZORRO = 0x33;

	/// <summary>
	/// The piccol o_ bloc k_ size
	/// </summary>
	static final PICCOLO_BLOCK_SIZE = 0x08;
	/// <summary>
	/// The piccol o_ ke y_ size
	/// </summary>
	static final PICCOLO_KEY_SIZE = 0x0A;

	/// <summary>
	/// The lbloc k_ bloc k_ size
	/// </summary>
	static final LBLOCK_BLOCK_SIZE = 0x08;
	/// <summary>
	/// The lbloc k_ ke y_ size
	/// </summary>
	static final LBLOCK_KEY_SIZE = 0x0A;

	/// <summary>
	/// The rectangl e_ bloc k_ size
	/// </summary>
	static final RECTANGLE_BLOCK_SIZE = 0x08;
	/// <summary>
	/// The rectangl e_ ke y_ size
	/// </summary>
	static final RECTANGLE_KEY_SIZE = 0x0A;

	/// <summary>
	/// The zorr o_ bloc k_ size
	/// </summary>
	static final ZORRO_BLOCK_SIZE = 0x08;
	/// <summary>
	/// The zorr o_ ke y_ size
	/// </summary>
	static final ZORRO_KEY_SIZE = 0x0A;

	/// <summary>
	/// The twin e_ bloc k_ size
	/// </summary>
	static final TWINE_BLOCK_SIZE = 0x08;
	/// <summary>
	/// The twin e_ ke y_ size
	/// </summary>
	static final TWINE_KEY_SIZE = 0x0A;

	/// <summary>
	/// The has h_ kecca K_160
	/// </summary>
	static final HASH_KECCAK_160 = 0x40;
	/// <summary>
	/// The has h_ kecca K_160_ bloc k_ size
	/// </summary>
	static final HASH_KECCAK_160_BLOCK_SIZE = 0x14;
	/// <summary>
	/// The has h_ kecca K_R144C256
	/// </summary>
	static final HASH_KECCAK_r144c256 = 0x41;
	/// <summary>
	/// The has h_ kecca K_R144C256_ bloc k_ size
	/// </summary>
	static final HASH_KECCAK_r144c256_BLOCK_SIZE = 0x20;
	/// <summary>
	/// The has h_ kecca K_R128C272
	/// </summary>
	static final HASH_KECCAK_r128c272 = 0x42;
	/// <summary>
	/// The has h_ kecca K_R128C272_ bloc k_ size
	/// </summary>
	static final HASH_KECCAK_r128c272_BLOCK_SIZE = 0x22;
	/// <summary>
	/// The has h_ kecca K_R544C256
	/// </summary>
	static final HASH_KECCAK_r544c256 = 0x43;
	/// <summary>
	/// The has h_ kecca K_R544C256_ bloc k_ size
	/// </summary>
	static final HASH_KECCAK_r544c256_BLOCK_SIZE = 0x44;
	/// <summary>
	/// The has h_ kecca K_R512C288
	/// </summary>
	static final HASH_KECCAK_r512c288 = 0x44;
	/// <summary>
	/// The has h_ kecca K_R512C288_ bloc k_ size
	/// </summary>
	static final HASH_KECCAK_r512c288_BLOCK_SIZE = 0x40;
	/// <summary>
	/// The has h_ kecca K_R256C544
	/// </summary>
	static final HASH_KECCAK_r256c544 = 0x46;
	/// <summary>
	/// The has h_ kecca K_R256C544_ bloc k_ size
	/// </summary>
	static final HASH_KECCAK_r256c544_BLOCK_SIZE = 0x44;

public:
	/// <summary>
	/// Gets the instance.
	/// </summary>
	/// <param name="CIPHER">The cipher.</param>
	/// <returns>CipherOffCard *.</returns>
	static CipherOffCard* getInstance(final CIPHER = 0x00);
	/// <summary>
	/// Generates the random.
	/// </summary>
	/// <param name="size">The size.</param>
	/// <returns>vArray &.</returns>
	vArray& generateRandom(size_t size);
	/// <summary>
	/// Gets the key.
	/// </summary>
	/// <param name="cipher">The cipher.</param>
	/// <returns>Apdu *.</returns>
	Apdu* getKey(final cipher = CURRENT_CIPHER);
	/// <summary>
	/// Gets the block.
	/// </summary>
	/// <param name="Mode">The mode.</param>
	/// <param name="cipher">The cipher.</param>
	/// <returns>Apdu *.</returns>
	Apdu* getBlock(final Mode, final cipher = CURRENT_CIPHER);
	/// <summary>
	/// Gets the type.
	/// </summary>
	/// <returns>int.</returns>
	int getType();

};

