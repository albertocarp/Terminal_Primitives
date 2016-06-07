#pragma once
#ifdef WIN32
#undef UNICODE
#endif

#include<winscard.h>
#include<vector>
#include<string>
#include<exception>
#include<iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <random>
#include <climits>
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>

typedef const byte  final;
typedef std::vector<std::string> Matrix;
typedef std::vector<byte> vArray;
typedef unsigned long word;

namespace BioAuth {

#define LOG 
	struct Config {
		final OFFSET_P1_CHAIN_BIO = 0x24;
		final OFFSET_P1_SET_PASSWORD = 0x25;
		final OFFSET_P1_SET_IDENTITY = 0x26;
		final OFFSET_P1_REGISTRATION = 0x27;
		final OFFSET_P1_ADDITIONAL_DATA = 0x28;
		final OFFSET_PI_SECOND_MESSAGE = 0x29;
		final OFFSET_P1_REGISTRATION_MESSAGE = 0x33;
		final OFFSET_P1_AUTH_FIRST_STEP = 0x34;
		final OFFSET_P1_AUTH_SECOND_STEP = 0x35;
		final OFFSET_P1_GET_PASSWORD = 0x40;
		final OFFSET_P1_GET_IDENTITY = 0x41;
		final OFFSET_P1_GET_ADITIONAL_FIELDS = 0x42;
		final OFFSET_P1_GET_BIO = 0x43;
		final OFFSET_P1_GET_GALOIS = 0x44;
		final OFFSET_P1_AUTH_PASSWORD = 0x30;
		final OFFSET_P1_AUTH_IDENTITY = 0x31;
		final OFFSET_P1_CHAIN_AUTH = 0x32;
		final OFFSET_P2_CONFIG_DATA = (byte)0x27;
		final OFFSET_P2_GALOIS_FIELD = (byte)0x28;
		final OFFSET_P2_COMPUTE = (byte)0x29;
		final OFFSET_P2_MORE_FRAGMENTS = 0x00;
		final OFFSET_P2_LAST_FRAGMENT = 0x01;
		final OFFSET_P1_GET_P = (byte)0x44;
		final OFFSET_P1_GET_Q = (byte)0x45;
		final OFFSET_P1_GET_S3 = (byte)0x46;
		final OFFSET_P1_GET_S1 = (byte)0x47;
		final OFFSET_P1_GET_SK = (byte)0x48;

		char* LENGTH;
		char*M;
		char*T;
		char*K;
		short LENGTH_SHORT = 384;
		short M_SHORT = 9;
		short T_SHORT = 52;
		short D_SHORT = 2 * T_SHORT + 1;
		short K_SHORT = LENGTH_SHORT - M_SHORT*T_SHORT;
		size_t HASH_SIZE = 0x14;
		size_t SIZE_P = 128;
		size_t SIZE_Q = 128;
		size_t SIZE_MESSAGES = 128;
		byte* GROUP_P_LOCAL = (byte*)malloc(128);

		static Config* p_Instance;
	public:
		static Config* Instance()
		{
			if (p_Instance == nullptr)
				p_Instance = new Config();
			return p_Instance;
		}
		Config()
		{
			LENGTH = new char[2];
			LENGTH[0] = 0x01; 
			LENGTH[1] = (char)0x82; 

			M = new char[2];
			M[0] = 0x00;
			M[1] = 0x1C;

			T = new char[2];
			T[0] = 0x00;
			T[1] = 0x34;

			K = new char[2];
			K[0] = 0x00;
			K[1] = 0x18;

			byte GROUP_P[] = {
				(byte)0xD2, (byte)0x1A, (byte)0xE8, (byte)0xD6, (byte)0x6E, (byte)0x6C, (byte)0x6B, (byte)0x3C, (byte)0xED, (byte)0x0E,
				(byte)0xB3, (byte)0xDF, (byte)0x1A, (byte)0x26, (byte)0xC9, (byte)0x1B, (byte)0xDE, (byte)0xED, (byte)0x01, (byte)0x3C, (byte)0x17,
				(byte)0xD8, (byte)0x49, (byte)0xD3, (byte)0x0E, (byte)0xC3, (byte)0x09, (byte)0x81, (byte)0x3E, (byte)0x4D, (byte)0x37, (byte)0x99, (byte)0xF2,
				(byte)0x6D, (byte)0xB0, (byte)0xD4, (byte)0x94, (byte)0xE8, (byte)0x2E, (byte)0xC6, (byte)0x1E, (byte)0xA9, (byte)0xFD, (byte)0xC7, (byte)0x0B,
				(byte)0xB5, (byte)0xCB, (byte)0xCA, (byte)0xF2, (byte)0xE5, (byte)0xF1, (byte)0x8A, (byte)0x83, (byte)0x64, (byte)0x94, (byte)0xF5, (byte)0x8E,
				(byte)0x67, (byte)0xC6, (byte)0xD6, (byte)0x16, (byte)0x48, (byte)0x0C, (byte)0x37, (byte)0xA7, (byte)0xF2, (byte)0x30, (byte)0x61, (byte)0x01,
				(byte)0xFC, (byte)0x9F, (byte)0x0F, (byte)0x47, (byte)0x68, (byte)0xF9, (byte)0xC9, (byte)0x79, (byte)0x3C, (byte)0x2B, (byte)0xE1, (byte)0x76,
				(byte)0xB0, (byte)0xB7, (byte)0xC9, (byte)0x79, (byte)0xB4, (byte)0x06, (byte)0x5D, (byte)0x3E, (byte)0x83, (byte)0x56, (byte)0x86, (byte)0xA3,
				(byte)0xF0, (byte)0xB8, (byte)0x42, (byte)0x0C, (byte)0x68, (byte)0x34, (byte)0xCB, (byte)0x17, (byte)0x93, (byte)0x03, (byte)0x86, (byte)0xDE,
				(byte)0xDA, (byte)0xB2, (byte)0xB0, (byte)0x7D, (byte)0xD4, (byte)0x73, (byte)0x44, (byte)0x9A, (byte)0x48, (byte)0xBA, (byte)0xAB, (byte)0x31,
				(byte)0x62, (byte)0x86, (byte)0xB4, (byte)0x21, (byte)0x05, (byte)0x24, (byte)0x75, (byte)0xD1, (byte)0x34, (byte)0xCD, (byte)0x3B };
			memcpy(GROUP_P_LOCAL, GROUP_P, 128);
		}

	};
}
