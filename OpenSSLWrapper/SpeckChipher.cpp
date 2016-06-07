#include "SpeckChipher.h"


SpeckChipher::SpeckChipher(void)
{

}
SpeckChipher::~SpeckChipher(void)
{

}
/*
  private methods B64_K128
*/

#pragma region Private_Methods
	uint32_t* SpeckChipher_B64_K128::_Key_Expand(uint32_t* block)
		{
			std::reverse(block ,block + this->_Key_Words);
			uint32_t* out = (uint32_t*)malloc((this->_Rounds) *sizeof(uint32_t));
			out[0] = block[0];
			uint32_t i(0);
			for(i;i<this->_Rounds-1;i++)
			{
				size_t index = 1+i%(this->_Key_Words-1);
				block[index] = Util::RCS32(block[index],8);
				block[index] += block[0];
				block[index] ^= uint32_t(i);
				block[0]  = Util::LCS32(block[0],3);
				block[0] ^= block[index];
				out[i+1] = block[0];
			}
			return out;
		}
	uint32_t* SpeckChipher_B64_K128::_Block_Encrypt(uint32_t * block,uint32_t*_Key_Expansion)
	{
			for(int i=0;i<this->_Rounds;i++)
			{
				block[0] = Util::RCS32(block[0],8);
				block[0] += block[1];
				block[0] ^= _Key_Expansion[i];

				block[1] = Util::LCS32(block[1],3);
				block[1] ^= block[0];
			}
			return block;
	}
	uint32_t* SpeckChipher_B64_K128::_Block_Decrypt(uint32_t * encBlock,uint32_t* _Key_Expansion)
	{
			for(int i = this->_Rounds;i > 0 ;i--)
			{
				encBlock[1] ^= encBlock[0];
				encBlock[1] = Util::RCS32(encBlock[1],3);

				encBlock[0] ^= _Key_Expansion[i-1];
				encBlock[0] -= encBlock[1];
				encBlock[0]  = Util::LCS32(encBlock[0],8);
			}
			return encBlock;
	}

	uint32_t* SpeckChipher_B64_K128::_get_Blk(unsigned char* pData)
	{
			uint32_t* data = (uint32_t*)malloc(2 * sizeof(uint32_t));
			Util::ToUInt32(pData,data[0]);
			Util::ToUInt32(pData+4,data[1]);
			return data;
	}
	unsigned char* SpeckChipher_B64_K128::_get_Array(uint32_t* blk)
	{
			unsigned char* buffer = (unsigned char*)malloc(Speck_Block_Len);
			Util::ToCharArray(&blk[0],buffer);
			Util::ToCharArray(&blk[1],buffer + 4);
			return buffer;
	}

#pragma endregion

/*---Only for testing--*/

uint32_t* SpeckChipher_B64_K128::EncryptBlock32(uint32_t* in_blk,Output_Target target,char* _File)
{
	uint32_t * out_blk = this->_Block_Encrypt(in_blk,this->_KEY_Expansion);
	unsigned char* asci_Enc = _get_Array(out_blk);
	if(target == Output_Target::STDOUT)
		Util::_print_hexa_buffer(asci_Enc,Speck_Block_Len);
	else
	{
		Util::_write_to_file(_File,asci_Enc,Speck_Block_Len);
	}
	return out_blk;
}
uint64_t* SpeckChipher_B64_K128::EncryptBlock64(uint64_t*,Output_Target target,char* _File)
{
	throw new exception("Eroare functie.Acces interzis");
}

void SpeckChipher_B64_K128::BuildChipher(uint32_t* block)
{
	this->_KEY_Expansion = this->_Key_Expand(block);
}
uint32_t* SpeckChipher_B64_K128::DecryptBlock32(uint32_t* in_blk,Output_Target target,char* _File)
{
	uint32_t * out_blk = this->_Block_Decrypt(in_blk,this->_KEY_Expansion);
	unsigned char* asci_Enc = _get_Array(out_blk);
	if(target == Output_Target::STDOUT)
		Util::_print_hexa_buffer(asci_Enc,Speck_Block_Len);
	else
	{
		Util::_write_to_file(_File,asci_Enc,Speck_Block_Len);
	}
	return out_blk;
}
uint64_t* SpeckChipher_B64_K128::DecryptBlock64(uint64_t* in_blk,Output_Target target,char* File)
{
   throw new exception("functie interzisa");
}
void  SpeckChipher_B64_K128::BuildChipher  (uint64_t* block)
{
	 throw new exception("functie interzisa");
}

/*public method B64_K128*/

void SpeckChipher_B64_K128::BuildChipher(const char* key)
{
	if(strlen(key) != (this->_Key_Words * this->_Word_Size)/OCTET_SIZE )
		throw new exception("Cheie incompatibila.Cheia trebuie sa aiba 16 bytes");
	uint32_t vec[4];
	this->_Key = (unsigned char*)_strdup(key);
	for(int i=0;i<3;i++)
	{
	  Util::ToUInt32(this->_Key + 4*i,vec[i]);
	}
	this->_KEY_Expansion = this->_Key_Expand(vec);
}
void SpeckChipher_B64_K128::Encrypt(char* InputFile,char* OutputFile)
{
	   unsigned char* clearData = nullptr;
	   unsigned int clearLen = 0;
	   unsigned char* encData = nullptr;
	   unsigned int encLen = 0;

	   int status = Util::_read_from_file(InputFile,&clearData,&clearLen);
	   unsigned char* pClearData = clearData;

	   if(status != STATUS_OK)
		   throw new exception("Eroare citire fisier");

	   encData = (unsigned char*)malloc(0);

	   while(clearLen >= Speck_Block_Len)
	   {
		   uint32_t* block =_get_Blk(pClearData);
		   uint32_t* oublk = this->_Block_Encrypt(block,this->_KEY_Expansion);
		   unsigned char* outBlkArray = this->_get_Array(oublk);
		   encData = (unsigned char*)realloc(encData,encLen + Speck_Block_Len);
		   memcpy(encData + encLen,outBlkArray,Speck_Block_Len);
		   encLen += Speck_Block_Len;
		   pClearData += Speck_Block_Len;
		   clearLen -= Speck_Block_Len;
	   }
	   //avem padding?
	   if(clearLen != 0)
	   {
		   unsigned char blkPad[Speck_Block_Len];
		   memcpy(blkPad,pClearData,clearLen);
		   memset(blkPad + clearLen,Speck_Block_Len - clearLen,Speck_Block_Len - clearLen);
		   uint32_t* block =_get_Blk(blkPad);
		   uint32_t* oublk = this->_Block_Encrypt(block,this->_KEY_Expansion);
		   unsigned char* outBlkArray = this->_get_Array(oublk);
		   encData = (unsigned char*)realloc(encData,encLen + Speck_Block_Len);
		   memcpy(encData + encLen,outBlkArray,Speck_Block_Len);
		   encLen += Speck_Block_Len;
	   }
	   status = Util::_write_to_file(OutputFile,encData,encLen);
	   if(status != STATUS_OK)
		   throw new exception("Eroare scriere fisier");
}
void SpeckChipher_B64_K128::Decrypt(char* EncFile,char* OutputFile) 
{
	   unsigned char* clearData = nullptr;
	   unsigned int clearLen = 0;
	   unsigned char* encData = nullptr;
	   unsigned int encLen = 0;
	   int status = Util::_read_from_file(EncFile,&encData,&encLen);
	   unsigned char* pEncData = encData;
	   if(status != STATUS_OK)
		   throw new exception("Eroare citire fisier");
	   clearData = (unsigned char*)malloc(0);

	   while(encLen >= Speck_Block_Len)
	   {
		   uint32_t* block =_get_Blk(pEncData);
		   uint32_t* oublk = this->_Block_Decrypt(block,this->_KEY_Expansion);
		   unsigned char* outBlkArray = this->_get_Array(oublk);
		   clearData = (unsigned char*)realloc(clearData,clearLen + Speck_Block_Len);
		   memcpy(clearData + clearLen,outBlkArray,Speck_Block_Len);
		   clearLen += Speck_Block_Len;
		   pEncData += Speck_Block_Len;
		   encLen -= Speck_Block_Len;
	   }
	   if(encLen != 0)
	   {
		   unsigned char blkPad[Speck_Block_Len];
		   memcpy(blkPad,pEncData,encLen);
		   memset(blkPad + encLen,Speck_Block_Len - encLen,Speck_Block_Len - encLen);
		   uint32_t* block =_get_Blk(blkPad);
		   uint32_t* oublk = this->_Block_Encrypt(block,this->_KEY_Expansion);
		   unsigned char* outBlkArray = this->_get_Array(oublk);
		   clearData = (unsigned char*)realloc(clearData,clearLen + Speck_Block_Len);
		   memcpy(clearData + clearLen,outBlkArray,Speck_Block_Len);
		   clearData += Speck_Block_Len;
	   }
	   status = Util::_write_to_file(OutputFile,clearData,clearLen);
	   if(status != STATUS_OK)
		   throw new exception("Eroare scriere fisier");
}
/*
   public method B64_K128
*/

#pragma region Private_Method_128_256
uint64_t*	SpeckChipher_B128_K256::   _Key_Expand(uint64_t* block)
 {
	 	std::reverse(block ,block + this->_Key_Words);
	   uint64_t* out = (uint64_t*)malloc((this->_Rounds + 1) *sizeof(uint64_t));
		out[0] = block[0];
		uint64_t i(0);
		for(i;i<this->_Rounds-1;i++)
		{
			size_t index = 1+i%(this->_Key_Words-1);
			block[index] = Util::RCS64(block[index],8);
			block[index] += block[0];
			block[index] ^= uint64_t(i);
			block[0]  = Util::LCS64(block[0],3);
			block[0] ^= block[index];
			out[i+1] = block[0];
		}
		return out;
 }
uint64_t*	SpeckChipher_B128_K256::   _Block_Encrypt(uint64_t * block,uint64_t*_Key_Expansion)
{
   for(int i=0;i<this->_Rounds;i++)
		{
			block[0] = Util::RCS64(block[0],8);
			block[0] += block[1];
			block[0] ^= _Key_Expansion[i];
			block[1] = Util::LCS64(block[1],3);
			block[1] ^= block[0];
		}
		return block;
}
uint64_t*	SpeckChipher_B128_K256::   _Block_Decrypt(uint64_t * encBlock,uint64_t* _Key_Expansion)
{
	for(int i = this->_Rounds;i > 0 ;i--)
		{
			encBlock[1] ^= encBlock[0];
			encBlock[1] = Util::RCS64(encBlock[1],3);
			encBlock[0] ^= _Key_Expansion[i-1];
			encBlock[0] -= encBlock[1];
			encBlock[0]  = Util::LCS64(encBlock[0],8);
		}
		return encBlock;
}
#pragma endregion

uint64_t*	SpeckChipher_B128_K256::    _get_Blk(unsigned char* pData)
{
	  uint64_t* data = (uint64_t*)malloc(2 * sizeof(uint64_t));
	  Util::ToUInt64(pData,data[0]);
	  Util::ToUInt64(pData+8,data[1]);
	  return data;
}
unsigned char*  SpeckChipher_B128_K256::_get_Array(uint64_t* blk)
{
	    unsigned char* buffer = (unsigned char*)malloc(Speck_Block_Len);
		Util::ToCharArray(&blk[0],buffer);
		Util::ToCharArray(&blk[1],buffer + 8);
		return buffer;
}

void SpeckChipher_B128_K256::BuildChipher(const char* key)
{
	if(strlen(key) != (this->_Key_Words * this->_Word_Size)/OCTET_SIZE )
		throw new exception("Cheie incompatibila.Cheia trebuie sa aiba 32 bytes");
	uint64_t vec[4];
	this->_Key = (unsigned char*)_strdup(key);
	for(int i=0;i<3;i++)
	{
		Util::ToUInt64(this->_Key + 4*i,vec[i]);
	}
	this->_KEY_Expansion = this->_Key_Expand(vec);
}
void SpeckChipher_B128_K256::Encrypt(char* InputFile,char* OutputFile)
{
	  unsigned char* clearData = nullptr;
	   unsigned int clearLen = 0;
	   unsigned char* encData = nullptr;
	   unsigned int encLen = 0;

	   int status = Util::_read_from_file(InputFile,&clearData,&clearLen);
	   unsigned char* pClearData = clearData;

	   if(status != STATUS_OK)
		   throw new exception("Eroare citire fisier");

	   encData = (unsigned char*)malloc(0);

	   while(clearLen >= Speck_Block_Len)
	   {
		   uint64_t* block =_get_Blk(pClearData);
		   uint64_t* oublk = this->_Block_Encrypt(block,this->_KEY_Expansion);
		   unsigned char* outBlkArray = this->_get_Array(oublk);
		   encData = (unsigned char*)realloc(encData,encLen + Speck_Block_Len);
		   memcpy(encData + encLen,outBlkArray,Speck_Block_Len);
		   encLen += Speck_Block_Len;
		   pClearData += Speck_Block_Len;
		   clearLen -= Speck_Block_Len;
	   }
	   //avem padding?
	   if(clearLen != 0)
	   {
		   unsigned char blkPad[Speck_Block_Len];
		   memcpy(blkPad,pClearData,clearLen);
		   memset(blkPad + clearLen,Speck_Block_Len - clearLen,Speck_Block_Len - clearLen);
		   uint64_t* block =_get_Blk(blkPad);
		   uint64_t* oublk = this->_Block_Encrypt(block,this->_KEY_Expansion);
		   unsigned char* outBlkArray = this->_get_Array(oublk);
		   encData = (unsigned char*)realloc(encData,encLen + Speck_Block_Len);
		   memcpy(encData + encLen,outBlkArray,Speck_Block_Len);
		   encLen += Speck_Block_Len;
	   }
	   status = Util::_write_to_file(OutputFile,encData,encLen);
	   if(status != STATUS_OK)
		   throw new std::exception("Eroare scriere fisier");
}
void SpeckChipher_B128_K256::Decrypt(char* EncFile,char* OutputFile) 
{
	   unsigned char* clearData = nullptr;
	   unsigned int clearLen = 0;
	   unsigned char* encData = nullptr;
	   unsigned int encLen = 0;
	   int status = Util::_read_from_file(EncFile,&encData,&encLen);
	   unsigned char* pEncData = encData;
	   if(status != STATUS_OK)
		   throw new std::exception("Eroare citire fisier");
	   clearData = (unsigned char*)malloc(0);

	   while(encLen >= Speck_Block_Len)
	   {
		   uint64_t* block =_get_Blk(pEncData);
		   uint64_t* oublk = this->_Block_Decrypt(block,this->_KEY_Expansion);
		   unsigned char* outBlkArray = this->_get_Array(oublk);
		   clearData = (unsigned char*)realloc(clearData,clearLen + Speck_Block_Len);
		   memcpy(clearData + clearLen,outBlkArray,Speck_Block_Len);
		   clearLen += Speck_Block_Len;
		   pEncData += Speck_Block_Len;
		   encLen -= Speck_Block_Len;
	   }
	   if(encLen != 0)
	   {
		   unsigned char blkPad[Speck_Block_Len];
		   memcpy(blkPad,pEncData,encLen);
		   memset(blkPad + encLen,Speck_Block_Len - encLen,Speck_Block_Len - encLen);
		   uint64_t* block =_get_Blk(blkPad);
		   uint64_t* oublk = this->_Block_Encrypt(block,this->_KEY_Expansion);
		   unsigned char* outBlkArray = this->_get_Array(oublk);
		   clearData = (unsigned char*)realloc(clearData,clearLen + Speck_Block_Len);
		   memcpy(clearData + clearLen,outBlkArray,Speck_Block_Len);
		   clearData += Speck_Block_Len;
	   }
	   status = Util::_write_to_file(OutputFile,clearData,clearLen);
	   if(status != STATUS_OK)
		   throw new std::exception("Eroare scriere fisier");
}

uint32_t* SpeckChipher_B128_K256::EncryptBlock32(uint32_t*,Output_Target target,char* File)
{
	throw new exception("Functie interzia");
}
uint32_t* SpeckChipher_B128_K256::DecryptBlock32(uint32_t*,Output_Target target,char* File)
{
	throw new exception("Functie interzia");
}

uint64_t* SpeckChipher_B128_K256::EncryptBlock64(uint64_t* in_blk,Output_Target target,char* File)
{
	uint64_t * out_blk = this->_Block_Encrypt(in_blk,this->_KEY_Expansion);
	unsigned char* asci_Enc = _get_Array(out_blk);
	if(target == Output_Target::STDOUT)
		Util::_print_hexa_buffer(asci_Enc,2*Speck_Block_Len);
	else
	{
		Util::_write_to_file(File,asci_Enc,2*Speck_Block_Len);
	}
	return out_blk;
}
uint64_t* SpeckChipher_B128_K256::DecryptBlock64(uint64_t* in_blk,Output_Target target,char* File)
{
	uint64_t * out_blk = this->_Block_Decrypt(in_blk,this->_KEY_Expansion);
	unsigned char* asci_Enc = _get_Array(out_blk);
	if(target == Output_Target::STDOUT)
		Util::_print_hexa_buffer(asci_Enc,Speck_Block_Len);
	else
	{
		Util::_write_to_file(File,asci_Enc,Speck_Block_Len);
	}
	return out_blk;
}
void      SpeckChipher_B128_K256::BuildChipher  (uint64_t* block)
{
	this->_KEY_Expansion = this->_Key_Expand(block);
}
void SpeckChipher_B128_K256::BuildChipher(uint32_t* block)
{
	throw new std::exception("eroare");
}