#pragma once
#include"Util.h"
class SpeckChipher
{
protected:
	unsigned char* _Key;
public:
	SpeckChipher(void);
	virtual ~SpeckChipher(void);

	virtual void BuildChipher(const char* key)=0;
	virtual void Encrypt(char* InputFile,char* OutputFile) = 0;
	virtual void Decrypt(char* EncFile,char* OutputFile) = 0;

	virtual uint32_t* EncryptBlock32(uint32_t*,Output_Target target,char* File = nullptr)=0;
	virtual uint32_t* DecryptBlock32(uint32_t*,Output_Target target,char* File = nullptr)=0;
	virtual void BuildChipher  (uint32_t* block)=0;
	/*just for testsing*/
	virtual uint64_t* EncryptBlock64(uint64_t*,Output_Target target,char* File = nullptr)=0;
	virtual uint64_t* DecryptBlock64(uint64_t*,Output_Target target,char* File = nullptr)=0;
    virtual void BuildChipher  (uint64_t* block)=0;
};

class SpeckChipher_B64_K128 : public SpeckChipher 
{
private:
	static const int _Rounds = Rounds::T_27;
	static const int _Key_Words = 4;
	static const int _Word_Size = 32;
	uint32_t* _KEY_Expansion;
private:

	uint32_t*		 _Key_Expand(uint32_t* block);
	uint32_t*		 _Block_Encrypt(uint32_t * block,uint32_t*_Key_Expansion);
	uint32_t*		 _Block_Decrypt(uint32_t * encBlock,uint32_t* _Key_Expansion);
	uint32_t*		 _get_Blk(unsigned char* pData);
	unsigned char*   _get_Array(uint32_t* blk);

public:

	virtual void BuildChipher(const char* key);
	virtual void Encrypt(char* InputFile,char* OutputFile);
	virtual void Decrypt(char* EncFile,char* OutputFile);

	/*---Only for testing--*/
	virtual uint32_t* EncryptBlock32(uint32_t*,Output_Target target,char* File = nullptr);
    virtual uint32_t* DecryptBlock32(uint32_t*,Output_Target target,char* File = nullptr);
	virtual void BuildChipher(uint32_t* block);
    virtual uint64_t* EncryptBlock64(uint64_t*,Output_Target target,char* File = nullptr);
	virtual uint64_t* DecryptBlock64(uint64_t*,Output_Target target,char* File = nullptr);
    virtual void BuildChipher  (uint64_t* block);

};

class SpeckChipher_B128_K256 : public SpeckChipher 
{
	private:
		static const int _Rounds = Rounds::T_34;
		static const int _Key_Words = 4;
		static const int _Word_Size = 64;
		uint64_t* _KEY_Expansion;
	private:
        uint64_t*		 _Key_Expand(uint64_t* block);
		uint64_t*		 _Block_Encrypt(uint64_t * block,uint64_t*_Key_Expansion);
	    uint64_t*		 _Block_Decrypt(uint64_t * encBlock,uint64_t* _Key_Expansion);
		uint64_t*		 _get_Blk(unsigned char* pData);
		unsigned char*   _get_Array(uint64_t* blk);
    public:
		virtual void BuildChipher(const char* key);
		virtual void Encrypt(char* InputFile,char* OutputFile);
		virtual void Decrypt(char* EncFile,char* OutputFile);
		virtual void BuildChipher(uint32_t* block);

		virtual uint32_t* EncryptBlock32(uint32_t*,Output_Target target,char* File = nullptr);
        virtual uint32_t* DecryptBlock32(uint32_t*,Output_Target target,char* File = nullptr);

		virtual uint64_t* EncryptBlock64(uint64_t*,Output_Target target,char* File = nullptr);
		virtual uint64_t* DecryptBlock64(uint64_t*,Output_Target target,char* File = nullptr);
		virtual void BuildChipher  (uint64_t* block);

};
