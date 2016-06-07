#pragma once
#include"link.h"
class BigNumber
{
private:

	BIGNUM *MyBn;

	BN_CTX *ctx;


public:
	BigNumber();
	// se poate initializa numar pozitiv si negativ
	BigNumber(int);
	BigNumber(unsigned int);
	BigNumber( char*);
	BigNumber(unsigned char*, unsigned int len);
	BigNumber(const BIGNUM *);
	BigNumber(BigNumber*);
	BIGNUM*_MyBn();
	//numarul de biti pe care sa fie generat  numarul prim ex: 512,1024,2048 etc
	void Generate_prime_BigNumber(int);
	//returneaza valoarea BigNumber-ului intr-un vector de char
	unsigned char* ToString(int&);
	unsigned char* ToString();
	//returneaza valoare in hexadecimal a BigNumber-ului
	char* ToHex();
	void set(int);
	void set(char*);
	void set(unsigned int);
	void set(const BIGNUM*);
	//ex BigNumber x; 
	//Bignumber a(3),b(4); 
	//x.sum(a,b);
	//in x se salveaza suma celor 2 numere; 
	//nu se pot face operatii de tipul a.sum(a+b);
	//la fel si pentru celelalte operatii sub,mul,div
	void sum(BigNumber& Numar_1);
	void sub (BigNumber& Numar_1);
	void mul (BigNumber& Numar_1);
	void div(BigNumber&Cat, BigNumber& Rest);
	//Radical din numarul introdus ca parametru , rezultatul se salveaza in MyBn
	int sqrt(BigNumber  Numar);
	//se apeleaza module pe valoarea proprie iar rezultatul este memorat in MyBn
	//ex: x.mod(new BigNumber(123));
	int mod(BigNumber Modul);
	//se apeleaza modulul pe primul numar introdus ca parametru, rezultatul este memorat in MyBn
	//ex: x.mod(  a ,  new BigNumber(123));
	int mod(BigNumber Numar, BigNumber Modul);
	//ridicare la putere  
	//ex: x.exp(new BigNumber(12331));
	int exp(BigNumber exponent);
	//ridicare la putere 
	//ex: x.exp( a , new BigNumber(12312) ); 
	int exp(BigNumber Numar, BigNumber exponent);
	//Se compara valoarea numarului introdus ca paramentru cu cea salvata in MyBn
	int cmp(BigNumber Numar);

	void modExp(BigNumber& exp,BigNumber& mod);
	unsigned int getLength();
	~BigNumber();

	// static methods
	static BigNumber fromRandom(int maxBits);
};

