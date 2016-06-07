#include "BigNumber.h"


BigNumber::BigNumber()
{
	ctx = BN_CTX_new();
	MyBn = BN_new();
}
BigNumber::BigNumber(int number)
{
	ctx = BN_CTX_new();
	MyBn = BN_new();
	if (number >= 0)
		BN_set_word(MyBn, number);
	else
		BN_set_negative(MyBn, number);
}
BigNumber::BigNumber(char*number)
{
	MyBn = BN_new();
	ctx = BN_CTX_new();
	BN_dec2bn(&MyBn, (char*)number);

}
BigNumber::BigNumber(unsigned char*number,unsigned int len)
{
	MyBn = BN_new();
	ctx = BN_CTX_new();
	BN_bin2bn(number,len,this->MyBn);
}
BigNumber::BigNumber(unsigned int number)
{
	MyBn = BN_new();
	ctx = BN_CTX_new();

	BN_set_bit(MyBn, number);

}
BigNumber::BigNumber(const BIGNUM *BN)
{
	ctx = BN_CTX_new();
	this->MyBn = BN_new();
	BN_copy(this->MyBn, BN);

};
BigNumber::BigNumber(BigNumber*BN){

	ctx = BN_CTX_new();
	this->MyBn = BN_new();
	BN_copy(this->MyBn, BN->_MyBn());

};;
void BigNumber::set(int number)
{
	MyBn = BN_new();

	if (number >= 0)
		BN_set_word(MyBn, number);
	else
		BN_set_negative(MyBn, number);

};
void BigNumber::set(char* number)
{
	MyBn = BN_new();
	BN_dec2bn(&MyBn, number);
};
void BigNumber::set(unsigned int number)
{
	MyBn = BN_new();
	BN_set_word(MyBn, number);

};
void BigNumber::set(const BIGNUM* number)
{
	this->MyBn = BN_new();
	BN_copy(this->MyBn, number);


};
void BigNumber::Generate_prime_BigNumber(int bits)
{
	MyBn = BN_new();
	BN_generate_prime(MyBn, bits, 0, NULL, NULL, NULL, NULL);

};
BigNumber::~BigNumber()
{
}
BigNumber BigNumber::fromRandom(int maxBits)
{
	unsigned char* data = (unsigned char*)malloc(maxBits);
	RAND_bytes(data, maxBits);
	return BigNumber(data, maxBits);
}
BIGNUM*BigNumber::_MyBn()
{
	return this->MyBn;

};

//operatii
void BigNumber::sum(BigNumber& BigNumber2)
{
	BigNumber* bn = new BigNumber();
	BN_set_word(bn->MyBn,0);
	BN_add(bn->MyBn, BigNumber2.MyBn,this->MyBn);
	this->MyBn = bn->MyBn;
};
void BigNumber::sub(BigNumber& BigNumber2)
{
	BigNumber* bn = new BigNumber();
	BN_set_word(bn->MyBn,0);
	BN_sub(bn->MyBn, BigNumber2.MyBn,this->MyBn);
	this->MyBn = bn->MyBn;

};
void BigNumber::mul(BigNumber& BigNumber1)
{
	BigNumber* bn = new BigNumber();
	BN_set_word(bn->MyBn,0);
	BN_mul(bn->MyBn, BigNumber1.MyBn,this->MyBn,this->ctx);
	this->MyBn = bn->MyBn;

};
void BigNumber::div(BigNumber& Impartitor, BigNumber& Rest)
{
    BigNumber* bn = new BigNumber();
	BN_set_word(bn->MyBn,0);
	BN_div(bn->MyBn,Rest.MyBn,this->MyBn,Impartitor.MyBn,this->ctx);
	this->MyBn = bn->MyBn;

};
int BigNumber::sqrt(BigNumber BigNumber)
{
	MyBn = BN_new();

	return BN_sqr(MyBn, BigNumber._MyBn(), ctx);

};
int BigNumber::mod(BigNumber mod)
{


	return BN_mod(MyBn, MyBn, mod._MyBn(), ctx);


};
int BigNumber::mod(BigNumber  bignumber, BigNumber mod)
{
	MyBn = BN_new();
	return BN_mod(MyBn, bignumber._MyBn(), mod._MyBn(), ctx);

};
int BigNumber::exp(BigNumber exp)
{
	ctx = BN_CTX_new();
	BIGNUM *x = BN_new();
	BN_copy(x, this->MyBn);
	this->MyBn = BN_new();
	return BN_exp(this->MyBn, x, exp._MyBn(), ctx);

};
int BigNumber::exp(BigNumber bignumber, BigNumber exp)
{
	return BN_exp(MyBn, bignumber._MyBn(), exp._MyBn(), ctx);
};
int BigNumber::cmp(BigNumber BigNumber)
{
	ToHex();
	return BN_cmp(this->MyBn, BigNumber._MyBn());
};

char*BigNumber::ToHex()
{
	return BN_bn2hex(this->MyBn);
}
unsigned char *BigNumber::ToString(int& length)
{
	unsigned char* BigNumber_char = (unsigned char*)malloc(getLength());
	length = BN_bn2bin(this->MyBn,BigNumber_char);
	return BigNumber_char;
}
unsigned char * BigNumber::ToString()
{
	unsigned char* BigNumber_char = (unsigned char*)malloc(getLength());
	BN_bn2bin(this->MyBn, BigNumber_char);
	return BigNumber_char;
}
unsigned int BigNumber::getLength()
{
	return BN_num_bytes(this->MyBn);
}
void BigNumber::modExp(BigNumber& exp,BigNumber& mod)
{
	BigNumber* bn = new BigNumber();
    BN_mod_exp(bn->MyBn,this->MyBn,exp.MyBn, mod._MyBn(), this->ctx);
	this->MyBn = bn->MyBn;
}