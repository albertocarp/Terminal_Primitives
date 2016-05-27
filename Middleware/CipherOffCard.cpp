#include "CipherOffCard.h"

byte CipherOffCard::CURRENT_CIPHER;


CipherOffCard::CipherOffCard( final CIPHER)
{
	CipherOffCard::CURRENT_CIPHER = CIPHER;
}
CipherOffCard * CipherOffCard::getInstance( final CIPHER)
{
	return new CipherOffCard(CIPHER);
}

vArray& CipherOffCard::generateRandom(size_t size)
{
	std::default_random_engine rng(myseed);
	std::uniform_int_distribution<int> rng_dist(0, 255);
	vArray* data = new vArray();
	for (size_t i = 0; i < size; i++)
	{
		data->push_back(static_cast<unsigned char>(rng_dist(rng)));
	}
	return *data;
}

Apdu * CipherOffCard::getKey(final cipher)
{
	vArray ret;
	byte cipherType;
	byte cipherSpeck;
	byte cipherSizeKey;
	switch (cipher)
	{
		case CIPHER_PICOLLO:
		{
			ret = generateRandom(PICCOLO_KEY_SIZE);
			cipherType = Configuration::CIPHER_LIGTWEIGHT;
			cipherSpeck = CIPHER_PICOLLO;
			cipherSizeKey = PICCOLO_KEY_SIZE;
			break;
		}
		case CIPHER_LBLOCK:
		{
			ret = generateRandom(LBLOCK_KEY_SIZE);
			cipherType = Configuration::CIPHER_LIGTWEIGHT;
			cipherSpeck = CIPHER_LBLOCK;
			cipherSizeKey = LBLOCK_KEY_SIZE;
			break;
		}
		case CIPHER_RECTANGLE:
		{
			ret = generateRandom(RECTANGLE_KEY_SIZE);
			cipherType = Configuration::CIPHER_LIGTWEIGHT;
			cipherSpeck = CIPHER_RECTANGLE;
			cipherSizeKey = RECTANGLE_KEY_SIZE;
			break;
		}
		case CIPHER_TWINE:
		{
			ret = generateRandom(TWINE_KEY_SIZE);
			cipherType = Configuration::CIPHER_LIGTWEIGHT;
			cipherSpeck = CIPHER_TWINE;
			cipherSizeKey = TWINE_KEY_SIZE;
			break;
		}
		case CIPHER_ZORRO:
		{
			ret = generateRandom(ZORRO_KEY_SIZE);
			cipherType = Configuration::CIPHER_LIGTWEIGHT;
			cipherSpeck = CIPHER_ZORRO;
			cipherSizeKey = ZORRO_KEY_SIZE;
			break;
		}
	};
	Apdu* apdu = new ApduRequest(Configuration::APPLICATION_CLA, Configuration::CIPHER_LIGTWEIGHT, MODE_GENERATE_KEY, cipherSpeck, cipherSizeKey, 0x00);
	apdu->wrap(&ret);
	return apdu;

}

Apdu * CipherOffCard::getBlock(final Mode,final cipher)
{
	vArray ret;
	byte cipherType;
	byte cipherSpeck;
	byte cipherSizeBlock;
	switch (cipher)
	{
		case CIPHER_PICOLLO:
		{
			ret = generateRandom(PICCOLO_BLOCK_SIZE);
			cipherType = Configuration::CIPHER_LIGTWEIGHT;
			cipherSpeck = CIPHER_PICOLLO;
			cipherSizeBlock = PICCOLO_BLOCK_SIZE;
			break;
		}
		case CIPHER_LBLOCK:
		{
			ret = generateRandom(LBLOCK_BLOCK_SIZE);
			cipherType = Configuration::CIPHER_LIGTWEIGHT;
			cipherSpeck = CIPHER_LBLOCK;
			cipherSizeBlock = LBLOCK_BLOCK_SIZE;
			break;
		}
		case CIPHER_RECTANGLE:
		{
			ret = generateRandom(RECTANGLE_BLOCK_SIZE);
			cipherType = Configuration::CIPHER_LIGTWEIGHT;
			cipherSpeck = CIPHER_RECTANGLE;
			cipherSizeBlock = RECTANGLE_BLOCK_SIZE;
			break;
		}
		case CIPHER_TWINE:
		{
			ret = generateRandom(TWINE_BLOCK_SIZE);
			cipherType = Configuration::CIPHER_LIGTWEIGHT;
			cipherSpeck = CIPHER_TWINE;
			cipherSizeBlock = TWINE_BLOCK_SIZE;
			break;
		}
		case CIPHER_ZORRO:
		{
			ret = generateRandom(ZORRO_BLOCK_SIZE);
			cipherType = Configuration::CIPHER_LIGTWEIGHT;
			cipherSpeck = CIPHER_ZORRO;
			cipherSizeBlock = ZORRO_BLOCK_SIZE;
			break;
		}
		case HASH_KECCAK_160:
		{
			ret = generateRandom(HASH_KECCAK_160);
			cipherType = Configuration::HASH_FUNCTIONS;
			cipherSpeck = HASH_KECCAK_160;
			cipherSizeBlock = ZORRO_BLOCK_SIZE;
			break;
		}
		case CIPHER_ZORRO:
		{
			ret = generateRandom(ZORRO_BLOCK_SIZE);
			cipherType = Configuration::CIPHER_LIGTWEIGHT;
			cipherSpeck = CIPHER_ZORRO;
			cipherSizeBlock = ZORRO_BLOCK_SIZE;
			break;
		}
	};
	Apdu* apdu = new ApduRequest(Configuration::APPLICATION_CLA, Configuration::CIPHER_LIGTWEIGHT, Mode, cipherSpeck, cipherSizeBlock, 0x00);
	apdu->wrap(&ret);
	return apdu;
}

int CipherOffCard::getType()
{
	switch (CURRENT_CIPHER)
	{
	case CIPHER_PICOLLO:
		return 0;
	case CIPHER_TWINE:
		return 0;
	case CIPHER_LBLOCK:
		return 0;
	default:
		break;
	}
	return 1;
}

CipherOffCard::~CipherOffCard()
{
}
