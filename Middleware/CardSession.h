#pragma once
#include "defs.h"
class CardSession
{
public:
	SCARDHANDLE hCard;
	DWORD dwActiveProtocol;
	DWORD readerIdx;
};
typedef std::vector<CardSession> vCardSessions;

