#pragma once

#define STATUS_OK					0
#define STATUS_ERR_BASE				500
#define STATUS_ERR_IO				(STATUS_ERR_BASE + 1)
#define STATUS_ERR_WRONG_LEN		(STATUS_ERR_BASE + 2)
#define _AES_BLOCK_LEN  16
#define _DES_BLOCK_LEN 8
#define Speck_Block_Len 8
typedef unsigned char uch;
typedef unsigned int uint;
#define OCTET_SIZE 8
using namespace std;
enum Error {
	_STATUS_OK = 0,
	_STATUS_ERROR_BASE = 500,
	_STATUS_ERROR_IO = 501,
	_STATUS_ERROR_WRONG_LEN = 502
};
enum Speck_Block_Type 
{
	B128K256,
	B64K128,
	B32K64
};
enum Rounds {
		T_22,
		T_23,
		T_26,
		T_27 = 27,
		T_28,
		T_29,
		T_34 = 34
	};
enum Output_Target {
	STDOUT,
	_FILE
};


#include <openssl/aes.h>
#include<stdint.h>
#include <openssl\des.h>
#include <openssl\bn.h>
#include <openssl\rsa.h>
#include  <openssl\pem.h>
#include <openssl\rand.h>
#include <openssl\evp.h>
#include <openssl/x509.h>
#include <openssl\rand.h>
#include <map>
#include <iostream>
