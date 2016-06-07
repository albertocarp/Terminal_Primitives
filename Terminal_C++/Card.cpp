#include "Card.h"



Card::Card()
{
	connect();
	detectReaders();
}

Card::~Card()
{
}

void Card::connect()
{
	long rv;
	rv = SCardEstablishContext(SCARD_SCOPE_USER, NULL, NULL, &this->internal.cardSession);
	if (rv != SCARD_S_SUCCESS)
		throw new SmartCardException("Failed to connect", rv);
	this->internal.nrReaders = 0;
	this->internal.vSessions.clear();
	this->internal.vToken = 0;
	this->internal.readers = nullptr;
}

void Card::connectToSlot(word idx)
{
	LONG rv;
	CardSession *  session = new CardSession();
	if (idx >= (size_t)this->internal.nrReaders)
		return throw new SmartCardException("Unknown reader");
	rv = SCardConnect(this->internal.cardSession, (*this->internal.readers)[idx].c_str(),
		SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1,
		&session->hCard, &session->dwActiveProtocol);

	if (rv != SCARD_S_SUCCESS)
	{
		delete session;
		throw new SmartCardException("Error connect",rv);
	}
	session->readerIdx = idx;
	this->internal.vSessions.push_back(*session);

	sessionHandle = this->internal.vSessions.size() - 1;
}

word Card::getLastSession()
{
	return sessionHandle;
}

Apdu& Card::sendApdu(const Apdu & apdu)
{
	byte pbRecvBuffer[258];
	long rv;
	if (this->sessionHandle >= this->internal.vSessions.size())
		throw new SmartCardException("There is no card inserted");
	SCARD_IO_REQUEST pioRecvPci;
	pioRecvPci.dwProtocol = (this->internal.vSessions)[sessionHandle].dwActiveProtocol;
	pioRecvPci.cbPciLength = sizeof(pioRecvPci);

	LPSCARD_IO_REQUEST pioSendPci;
	if ((this->internal.vSessions)[sessionHandle].dwActiveProtocol == SCARD_PROTOCOL_T1)
		pioSendPci = (LPSCARD_IO_REQUEST)SCARD_PCI_T1;
	else
		pioSendPci = (LPSCARD_IO_REQUEST)SCARD_PCI_T0;
	word expected_length = 258;//apdu.getExpectedLen();
	word send_length = apdu.getApduLength();
	CardSession session = (this->internal.vSessions).operator[](sessionHandle);
	byte * data = const_cast<Apdu&>(apdu).getNonConstantData();
	auto start = Timer::now();
	rv = SCardTransmit(session.hCard, pioSendPci,data,
		send_length, &pioRecvPci, pbRecvBuffer,&expected_length);
	auto end = Timer::now();
	auto duration = (double)(end - start);
	return *new ApduResponse(pbRecvBuffer, expected_length,duration);

}

void Card::detectReaders()
{
	long rv;
	word dwReaders = SCARD_AUTOALLOCATE;
	char* pmszReaders;
	char* pReader = NULL;
	word idx = 0;
	SCARDHANDLE hCard;
	word dwProtocol;
	rv = SCardListReaders(this->internal.cardSession, NULL, (LPTSTR)&pmszReaders, &dwReaders);
	if (rv != SCARD_S_SUCCESS)
		throw new SmartCardException("Error listing readers", rv);
	pReader = pmszReaders;
	dwReaders = 0;
	while ('\0' != *pReader)
	{
		dwReaders++;
		pReader = pReader + strlen(pReader) + 1;
	}
	if (this->internal.readers != nullptr)
	{
		std::vector<std::string> readers = *(this->internal.readers);
		for (auto it = readers.begin(); it != readers.end(); it++)
			(*it).clear();
		this->internal.readers->clear();
	}
	pReader = pmszReaders;
	this->internal.nrReaders = dwReaders;
	this->internal.readers = new Matrix();
	this->internal.vToken = new vBool();
	idx = 0;
	while ('\0' != *pReader)
	{
		std::string * str = new std::string();
		str->assign(pReader, pReader + strlen(pReader) + 1);
		(*this->internal.readers).push_back(*str);
		rv = SCardConnect(this->internal.cardSession, pReader, SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, &hCard, &dwProtocol);
		if (rv == SCARD_S_SUCCESS)
			(*this->internal.vToken).push_back(true);
		else
			(*this->internal.vToken).push_back(false);
		idx++;
		pReader = pReader + strlen(pReader) + 1;
	}
	rv = SCardFreeMemory(this->internal.cardSession, pmszReaders);
}


