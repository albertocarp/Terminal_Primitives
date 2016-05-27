#pragma once
#include "IssuerParameters.h"
#include "Card.h"
namespace Auth {

	final SET_EI = 0x50;
	final GET_EI = 0x51;

	final SET_PUBKEY = 0x52;
	final GET_PUBKEY = 0x53;

	final SET_UIDP = 0x54;
	final GET_UIDP = 0x55;

	final SET_PQG = 0x56;
	final GET_PQG = 0x57;

	final OFFET_P = 0x00;
	final OFFET_Q = 0x00;
	final OFFET_G = 0x00;

	final SET_UIDH = 0x58;
	final GET_UIDH = 0x59;

	final SET_PI = 0x67;
	final GET_PI = 0x68;

	final PRECOMPUTE_INPUTS = 0x64;

	class Issuer
	{
		Auth::IssuerParameters Parameters;
		Card card;
	public:
		void publicIssuerInfo(Auth::IssuerParameters parameters);
		Issuer(const Card & card,byte * selectedId,size_t selectdId_Size);
		Issuer();
		~Issuer();
	};
}

