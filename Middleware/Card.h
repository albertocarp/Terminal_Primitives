#pragma once
#include "ApduRequest.h"
#include "ApduResponse.h"
#include "SelectionApdu.h"
#include "SmartCardException.h"
#include "Timer.h"
#include "Configuration.h"
class Card
{
	static byte* DEFAULT_APPLET;
private:
	class CardInternal
	{
	public:
		SCARDCONTEXT  cardSession;
		Matrix*       readers;
		std::vector<bool>* vToken;
		int			  nrReaders;
		vCardSessions vSessions;
	}internal;
	word sessionHandle;
public:
	Card();
	~Card();
	void connect();
	void connectToSlot(word idx = 0);
	word getLastSession();
	Apdu& sendApdu(const Apdu& apdu);
	typedef std::vector<bool> vBool;
private:
	void detectReaders();
};

