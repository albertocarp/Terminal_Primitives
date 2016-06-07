#pragma once
#include "defs.h"
#include "Card.h"
class CipherOffCard
{
private:
	int myseed = 1234;
	CipherOffCard(final CIPHER);
	static byte CURRENT_CIPHER;
	~CipherOffCard();
public:

	static final MODE_ENCRYPT = 0x21;
	static final MODE_DECRYPT = 0x22;
	static final MODE_GENERATE_KEY = 0x23;
	
	static final CIPHER_PICOLLO = 0x34;
	static final CIPHER_TWINE	= 0x30;
	static final CIPHER_LBLOCK  = 0x32;
	static final CIPHER_RECTANGLE = 0x35;
	static final CIPHER_ZORRO = 0x33;

	static final PICCOLO_BLOCK_SIZE = 0x08;
	static final PICCOLO_KEY_SIZE = 0x0A;

	static final LBLOCK_BLOCK_SIZE = 0x08;
	static final LBLOCK_KEY_SIZE = 0x0A;

	static final RECTANGLE_BLOCK_SIZE = 0x08;
	static final RECTANGLE_KEY_SIZE = 0x0A;

	static final ZORRO_BLOCK_SIZE = 0x08;
	static final ZORRO_KEY_SIZE = 0x0A;

	static final TWINE_BLOCK_SIZE = 0x08;
	static final TWINE_KEY_SIZE = 0x0A;

	static final HASH_KECCAK_160 = 0x40;
	static final HASH_KECCAK_160_BLOCK_SIZE = 0x14;
	static final HASH_KECCAK_r144c256 = 0x41;
	static final HASH_KECCAK_r144c256_BLOCK_SIZE = 0x20;
	static final HASH_KECCAK_r128c272 = 0x42;
	static final HASH_KECCAK_r128c272_BLOCK_SIZE = 0x22;
	static final HASH_KECCAK_r544c256 = 0x43;
	static final HASH_KECCAK_r544c256_BLOCK_SIZE = 0x44;
	static final HASH_KECCAK_r512c288 = 0x44;
	static final HASH_KECCAK_r512c288_BLOCK_SIZE = 0x40;
	static final HASH_KECCAK_r256c544 = 0x46;
	static final HASH_KECCAK_r256c544_BLOCK_SIZE = 0x44;

public:
	static CipherOffCard* getInstance(final CIPHER = 0x00);
	vArray& generateRandom(size_t size);
	Apdu* getKey(final cipher = CURRENT_CIPHER);
	Apdu* getBlock(final Mode,final cipher = CURRENT_CIPHER);
	int getType();

};

