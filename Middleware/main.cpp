#ifdef WIN32
#undef UNICODE
#endif

#include<winscard.h>
#include<vector>
#include "Benchmark.h"
#include "CipherOffCard.h"
#include "CorelatedTestAttack.h"
#include "SingleAutomaticTest.h"
#include "Issuer.h"

/*
SCARD_CTX scardCtx;

LONG connect() {
	LONG rv;
	rv = SCardEstablishContext(SCARD_SCOPE_USER, NULL, NULL, &scardCtx.context);
	scardCtx.nrReaders = 0;
	scardCtx.readers = NULL;
	scardCtx.sessions.clear();
	scardCtx.token_present_at = NULL;
	return rv;
}

LONG releaseContext() {
	LONG rv;
	DWORD idx;
	rv = SCardReleaseContext(scardCtx.context);

	if (scardCtx.readers != NULL)
	{
		for (idx = 0; idx<scardCtx.nrReaders; idx++)
			free(scardCtx.readers[idx]);

		free(scardCtx.readers);
		free(scardCtx.token_present_at);
	}

	scardCtx.nrReaders = 0;
	scardCtx.readers = NULL;
	scardCtx.sessions.clear();
	scardCtx.token_present_at = NULL;

	return rv;
}


LONG MW_DetectReaders() {

	LONG rv;

	LPTSTR pmszReaders = NULL;
	LPTSTR pReader = NULL;
	DWORD dwReaders = SCARD_AUTOALLOCATE;
	DWORD idx = 0;
	SCARDHANDLE hCard;
	DWORD dwProtocol;

	rv = SCardListReaders(scardCtx.context, NULL, (LPTSTR)&pmszReaders, &dwReaders);
	if (rv != SCARD_S_SUCCESS)
		return rv;
	pReader = pmszReaders;
	dwReaders = 0;
	while ('\0' != *pReader)
	{
		dwReaders++;
		pReader = pReader + strlen(pReader) + 1;
	}


	if (scardCtx.readers != NULL)
	{
		for (idx = 0; idx<scardCtx.nrReaders; idx++)
			free(scardCtx.readers[idx]);

		free(scardCtx.readers);
		free(scardCtx.token_present_at);
	}


	pReader = pmszReaders;
	scardCtx.nrReaders = dwReaders;
	scardCtx.readers = (LPSTR*)calloc(dwReaders, sizeof(LPSTR));
	scardCtx.token_present_at = (bool*)calloc(dwReaders, sizeof(bool));
	idx = 0;
	while ('\0' != *pReader)
	{
		scardCtx.readers[idx] = (LPSTR)calloc(strlen(pReader) + 1, sizeof(CHAR));
		memcpy(scardCtx.readers[idx], pReader, strlen(pReader) + 1);


		rv = SCardConnect(scardCtx.context, pReader, SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, &hCard, &dwProtocol);
		if (rv == SCARD_S_SUCCESS)
			scardCtx.token_present_at[idx++] = true;
		else
			scardCtx.token_present_at[idx++] = false;


		pReader = pReader + strlen(pReader) + 1;
	}
	rv = SCardFreeMemory(scardCtx.context, pmszReaders);
	return rv;
}
LONG MW_ConnectToSlot(DWORD readerIdx, DWORD* pCardSessionIdx) {

	SCARDSESSION *session = (SCARDSESSION*)calloc(1, sizeof(SCARDSESSION));
	LONG rv;



	if (readerIdx >= scardCtx.nrReaders)
		return SCARD_E_UNKNOWN_READER;


	rv = SCardConnect(scardCtx.context, scardCtx.readers[readerIdx], SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, &session->hCard, &session->dwActiveProtocol);
	if (rv != SCARD_S_SUCCESS)
	{
		free(session);
		return rv;
	}

	session->readerIdx = readerIdx;
	scardCtx.sessions.push_back(*session);

	*pCardSessionIdx = scardCtx.sessions.size() - 1;


	return rv;
}







//MW_DisconnectFromSlot function *****************************************************************
LONG MW_DisconnectFromSlot(DWORD cardSessionIdx) {
	LONG rv;
	if (cardSessionIdx >= scardCtx.sessions.size())
		return SCARD_E_INVALID_HANDLE;

	rv = SCardDisconnect(scardCtx.sessions[cardSessionIdx].hCard, SCARD_LEAVE_CARD);
	if (rv != SCARD_S_SUCCESS)
		return rv;

	//scardCtx.sessions.erase(scardCtx.sessions.begin()+cardSessionIdx);

	return SCARD_S_SUCCESS;
}
//End of MW_DisconnectFromSlot function **********************************************************





//MW_TransmitAPDU function ************************************************************************************************
LONG MW_TransmitAPDU(BYTE* apdu, DWORD dwSendLength, BYTE* pbRecvBuffer, DWORD* pdwRecvLength, DWORD cardSessionIdx) {
	LONG rv;

	if (cardSessionIdx >= scardCtx.sessions.size())
		return SCARD_E_NO_SMARTCARD;

	SCARD_IO_REQUEST pioRecvPci;
	pioRecvPci.dwProtocol = scardCtx.sessions[cardSessionIdx].dwActiveProtocol;
	pioRecvPci.cbPciLength = sizeof(pioRecvPci);

	LPSCARD_IO_REQUEST pioSendPci;
	if (scardCtx.sessions[cardSessionIdx].dwActiveProtocol == SCARD_PROTOCOL_T1)
		pioSendPci = (LPSCARD_IO_REQUEST)SCARD_PCI_T1;
	else
		pioSendPci = (LPSCARD_IO_REQUEST)SCARD_PCI_T0;


	rv = SCardTransmit(scardCtx.sessions[cardSessionIdx].hCard, pioSendPci, apdu, dwSendLength, &pioRecvPci, pbRecvBuffer, pdwRecvLength);

	return rv;
}
//End of MW_TransmitAPDU function *****************************************************************************************





//MW_SelectApplet function ***************************************************************************************************
LONG MW_SelectApplet(DWORD cardSessionIdx, APDURESPONSE *response) {
	LONG rv;

	//BYTE selectAID[] = { 0x00,0xA4,0x04,0x00,0x0C,0xA0,0x00,0x00,0x00,0x63,0x50,0x4B,0x43,0x53,0x2D,0x31,0x35 };
	BYTE selectAID[] = { 0x00,0xA4,0x04,0x00,0x8,0xd0,0xd1,0xd2,0xd3,0xd4,0xd5,0x01,0x01 };
	DWORD sendLen = 13;
	BYTE pbRecvBuffer[258];
	DWORD recvLen = 255;

	rv = MW_TransmitAPDU(selectAID, sendLen, pbRecvBuffer, &recvLen, cardSessionIdx);

	memcpy(response->data, pbRecvBuffer, recvLen - 2);
	response->dataLen = recvLen - 2;
	response->status.SW1 = pbRecvBuffer[recvLen - 2];
	response->status.SW2 = pbRecvBuffer[recvLen - 1];

	return rv;
}
//End of MW_SelectApplet function ********************************************************************************************





//MW_DoSetup function *********************************************************************************************************
LONG MW_DoSetup(BYTE* soPIN, DWORD cardSessionIdx, APDURESPONSE* response) {

	LONG rv;
	BYTE setup[] = { 0x00,0x06,0x00,0x00,0x04,0x00,0x00,0x00,0x00 };
	DWORD sendLen = 9;
	BYTE pbRecvBuffer[258];
	DWORD recvLen = 258;

	DWORD i;
	for (i = 0; i<4; i++)
		setup[5 + i] = soPIN[i];




	rv = MW_TransmitAPDU(setup, sendLen, pbRecvBuffer, &recvLen, cardSessionIdx);
	if (rv == SCARD_W_RESET_CARD)
	{
		rv = SCardReconnect(scardCtx.sessions[cardSessionIdx].hCard, SCARD_SHARE_SHARED, scardCtx.sessions[cardSessionIdx].dwActiveProtocol, SCARD_RESET_CARD, &scardCtx.sessions[cardSessionIdx].dwActiveProtocol);
		if (rv != SCARD_S_SUCCESS)
			return rv;

		rv = MW_SelectApplet(cardSessionIdx, response);
		if (rv != SCARD_S_SUCCESS)
			return rv;


		rv = MW_TransmitAPDU(setup, sendLen, pbRecvBuffer, &recvLen, cardSessionIdx);
	}
	if (rv != SCARD_S_SUCCESS)
		return rv;


	memcpy(response->data, pbRecvBuffer, recvLen - 2);
	response->dataLen = recvLen - 2;
	response->status.SW1 = pbRecvBuffer[recvLen - 2];
	response->status.SW2 = pbRecvBuffer[recvLen - 1];



	return rv;
}
//End of MW_DoSetup function **************************************************************************************************





//MW_Login function *************************************************************************************************************
LONG MW_Login(BYTE* pin, DWORD userType, DWORD cardSessionIdx, APDURESPONSE* response) {

	LONG rv;

	BYTE verify[] = { 0x00,0x20,0x00,0x00,0x04,0x00,0x00,0x00,0x00 };
	DWORD sendLen = 9;
	BYTE pbRecvBuffer[258];
	DWORD recvLen = 258;

	DWORD i;
	for (i = 0; i<4; i++)
		verify[5 + i] = pin[i];

	verify[ISO7816_OFFSET_P2] = userType;

	rv = MW_TransmitAPDU(verify, sendLen, pbRecvBuffer, &recvLen, cardSessionIdx);
	if (rv == SCARD_W_RESET_CARD)
	{
		rv = SCardReconnect(scardCtx.sessions[cardSessionIdx].hCard, SCARD_SHARE_SHARED, scardCtx.sessions[cardSessionIdx].dwActiveProtocol, SCARD_RESET_CARD, &scardCtx.sessions[cardSessionIdx].dwActiveProtocol);
		if (rv != SCARD_S_SUCCESS)
			return rv;

		rv = MW_SelectApplet(cardSessionIdx, response);
		if (rv != SCARD_S_SUCCESS)
			return rv;

		rv = MW_TransmitAPDU(verify, sendLen, pbRecvBuffer, &recvLen, cardSessionIdx);
	}
	if (rv != SCARD_S_SUCCESS)
		return rv;


	memcpy(response->data, pbRecvBuffer, recvLen - 2);
	response->dataLen = recvLen - 2;
	response->status.SW1 = pbRecvBuffer[recvLen - 2];
	response->status.SW2 = pbRecvBuffer[recvLen - 1];


	return rv;
}
//End of MW_Login function *******************************************************************************************************






//MW_Logout function **************************************************************************************************************
LONG MW_Logout(DWORD userType, DWORD cardSessionIdx, APDURESPONSE* response) {
	LONG rv;

	BYTE MW_Logout[] = { 0x00,0x12,0x00,0x00,0x00 };
	DWORD sendLen = 5;
	BYTE pbRecvBuffer[258];
	DWORD recvLen = 258;

	MW_Logout[ISO7816_OFFSET_P2] = userType;

	rv = MW_TransmitAPDU(MW_Logout, sendLen, pbRecvBuffer, &recvLen, cardSessionIdx);
	if (rv == SCARD_W_RESET_CARD)
	{
		rv = SCardReconnect(scardCtx.sessions[cardSessionIdx].hCard, SCARD_SHARE_SHARED, scardCtx.sessions[cardSessionIdx].dwActiveProtocol, SCARD_RESET_CARD, &scardCtx.sessions[cardSessionIdx].dwActiveProtocol);
		if (rv != SCARD_S_SUCCESS)
			return rv;

		rv = MW_SelectApplet(cardSessionIdx, response);
		if (rv != SCARD_S_SUCCESS)
			return rv;

		rv = MW_TransmitAPDU(MW_Logout, sendLen, pbRecvBuffer, &recvLen, cardSessionIdx);
	}
	if (rv != SCARD_S_SUCCESS)
		return rv;


	memcpy(response->data, pbRecvBuffer, recvLen - 2);
	response->dataLen = recvLen - 2;
	response->status.SW1 = pbRecvBuffer[recvLen - 2];
	response->status.SW2 = pbRecvBuffer[recvLen - 1];


	return rv;
}
//End of MW_Logout function ********************************************************************************************************






//MW_UnlockAndChangeOwnerPIN function ************************************************************************************************
LONG MW_UnlockAndChangeOwnerPIN(BYTE* newPIN, DWORD cardSessionIdx, APDURESPONSE* response) {
	LONG rv;

	BYTE unblockChange[] = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
	DWORD sendLen = 9;
	BYTE pbRecvBuffer[258];
	DWORD recvLen = 258;

	unblockChange[ISO7816_OFFSET_INS] = INS_UNBLOCK_AND_CHANGE_OWNER_PIN;
	unblockChange[ISO7816_OFFSET_P2] = 0x04;
	unblockChange[ISO7816_OFFSET_LC] = 0x04;

	WORD i;
	for (i = 0; i<4; i++)
		unblockChange[5 + i] = newPIN[i];


	rv = MW_TransmitAPDU(unblockChange, sendLen, pbRecvBuffer, &recvLen, cardSessionIdx);
	if (rv == SCARD_W_RESET_CARD)
	{
		rv = SCardReconnect(scardCtx.sessions[cardSessionIdx].hCard, SCARD_SHARE_SHARED, scardCtx.sessions[cardSessionIdx].dwActiveProtocol, SCARD_RESET_CARD, &scardCtx.sessions[cardSessionIdx].dwActiveProtocol);
		if (rv != SCARD_S_SUCCESS)
			return rv;

		rv = MW_SelectApplet(cardSessionIdx, response);
		if (rv != SCARD_S_SUCCESS)
			return rv;

		rv = MW_TransmitAPDU(unblockChange, sendLen, pbRecvBuffer, &recvLen, cardSessionIdx);
	}
	if (rv != SCARD_S_SUCCESS)
		return rv;


	if ((pbRecvBuffer[recvLen - 2] == 0x69) && (pbRecvBuffer[recvLen - 1] == 0x82))
		return SCARD_W_CARD_NOT_AUTHENTICATED;


	memcpy(response->data, pbRecvBuffer, recvLen - 2);
	response->dataLen = recvLen - 2;
	response->status.SW1 = pbRecvBuffer[recvLen - 2];
	response->status.SW2 = pbRecvBuffer[recvLen - 1];


	return rv;
}
//End of MW_UnlockAndChangeOwnerPIN function ******************************************************************************************





//MW_GetRandomData function ************************************************************************************************************
LONG MW_GetRandomData(BYTE bytesNr, DWORD cardSessionIdx, APDURESPONSE* response) {
	LONG rv;

	BYTE getrandom[] = { 0x00,0x00,0x00,0x00,0x00 };
	BYTE getresponse[] = { 0x00,0xC0,0x00,0x00,0x00 };
	DWORD sendLen = 5;
	BYTE pbRecvBuffer[258];
	DWORD recvLen = 258;

	getrandom[ISO7816_OFFSET_INS] = INS_GET_RANDOM_DATA;
	getrandom[ISO7816_OFFSET_P1] = bytesNr;

	rv = MW_TransmitAPDU(getrandom, sendLen, pbRecvBuffer, &recvLen, cardSessionIdx);
	if (rv == SCARD_W_RESET_CARD)
	{
		rv = SCardReconnect(scardCtx.sessions[cardSessionIdx].hCard, SCARD_SHARE_SHARED, scardCtx.sessions[cardSessionIdx].dwActiveProtocol, SCARD_RESET_CARD, &scardCtx.sessions[cardSessionIdx].dwActiveProtocol);
		if (rv != SCARD_S_SUCCESS)
			return rv;

		rv = MW_SelectApplet(cardSessionIdx, response);
		if (rv != SCARD_S_SUCCESS)
			return rv;

		rv = MW_TransmitAPDU(getrandom, sendLen, pbRecvBuffer, &recvLen, cardSessionIdx);
	}
	if (rv != SCARD_S_SUCCESS)
		return rv;


	sendLen = 5;
	recvLen = 258;
	rv = MW_TransmitAPDU(getresponse, sendLen, pbRecvBuffer, &recvLen, cardSessionIdx);
	if (rv != SCARD_S_SUCCESS)
		return rv;

	sendLen = 5;
	getresponse[ISO7816_OFFSET_LC] = pbRecvBuffer[recvLen - 1];
	recvLen = 258;
	rv = MW_TransmitAPDU(getresponse, sendLen, pbRecvBuffer, &recvLen, cardSessionIdx);
	if (rv != SCARD_S_SUCCESS)
		return rv;

	memcpy(response->data, pbRecvBuffer, recvLen - 2);
	response->dataLen = recvLen - 2;
	response->status.SW1 = pbRecvBuffer[recvLen - 2];
	response->status.SW2 = pbRecvBuffer[recvLen - 1];

	return rv;
}
//End of MW_GetRandomData function ******************************************************************************************************





//MW_TransferDataPut function ************************************************************************************************************
LONG MW_TransferDataPut(BYTE* data, DWORD dataLength, DWORD cardSessionIdx, APDURESPONSE* response) {
	LONG rv = 0;

	BYTE transfer[260];
	DWORD sendLen = 260;
	BYTE pbRecvBuffer[258];
	DWORD recvLen = 258;

	DWORD fullChunks = dataLength / 255;
	DWORD lastChunkSize = dataLength % 255;
	DWORD i = 0;
	DWORD offset = 0;

	transfer[ISO7816_OFFSET_CLA] = CLA_CMD_CHAIN;
	transfer[ISO7816_OFFSET_INS] = INS_TRANSFER_DATA_PUT;
	transfer[ISO7816_OFFSET_P1] = (BYTE)((dataLength >> 8) & 0xFF);
	transfer[ISO7816_OFFSET_P2] = (BYTE)(dataLength & 0xFF);
	transfer[ISO7816_OFFSET_LC] = 0xff;
	for (i = 0; i<fullChunks; i++)
	{
		memcpy(transfer + ISO7816_OFFSET_CDATA, data + offset, 255);
		offset += 255;
		sendLen = 260;
		recvLen = 258;
		rv = MW_TransmitAPDU(transfer, sendLen, pbRecvBuffer, &recvLen, cardSessionIdx);
		if ((rv == SCARD_W_RESET_CARD) && (i == 0))
		{
			rv = SCardReconnect(scardCtx.sessions[cardSessionIdx].hCard, SCARD_SHARE_SHARED, scardCtx.sessions[cardSessionIdx].dwActiveProtocol, SCARD_RESET_CARD, &scardCtx.sessions[cardSessionIdx].dwActiveProtocol);
			if (rv != SCARD_S_SUCCESS)
				return rv;

			rv = MW_SelectApplet(cardSessionIdx, response);
			if (rv != SCARD_S_SUCCESS)
				return rv;

			rv = MW_TransmitAPDU(transfer, sendLen, pbRecvBuffer, &recvLen, cardSessionIdx);
		}
		if (rv != SCARD_S_SUCCESS)
			return rv;
	}

	transfer[ISO7816_OFFSET_CLA] = CLA;
	transfer[ISO7816_OFFSET_LC] = (BYTE)lastChunkSize;
	sendLen = lastChunkSize + 5;
	recvLen = 258;
	memcpy(transfer + ISO7816_OFFSET_CDATA, data + offset, lastChunkSize);

	rv = MW_TransmitAPDU(transfer, sendLen, pbRecvBuffer, &recvLen, cardSessionIdx);
	if (rv != SCARD_S_SUCCESS)
		return rv;

	memcpy(response->data, pbRecvBuffer, recvLen - 2);
	response->dataLen = recvLen - 2;
	response->status.SW1 = pbRecvBuffer[recvLen - 2];
	response->status.SW2 = pbRecvBuffer[recvLen - 1];


	return rv;
}
//End of MW_TransferDataPut function *****************************************************************************************************





//MW_TransferDataGet function *************************************************************************************************************
LONG MW_TransferDataGet(BYTE** data, DWORD *dataLength, DWORD cardSessionIdx, APDURESPONSE* response) {
	LONG rv;

	BYTE transfer[] = { 0x00,0x00,0x00,0x00,0x00 };
	BYTE getrsp[] = { 0x00,0x00,0x00,0x00,0x00 };
	DWORD sendLen = 5;
	BYTE pbRecvBuffer[258];
	DWORD recvLen = 258;

	DWORD totalLen = 0;
	bool moreData = true;

	transfer[ISO7816_OFFSET_CLA] = CLA;
	transfer[ISO7816_OFFSET_INS] = INS_TRANSFER_DATA_GET;
	getrsp[ISO7816_OFFSET_INS] = INS_GET_RESPONSE;

	rv = MW_TransmitAPDU(transfer, sendLen, pbRecvBuffer, &recvLen, cardSessionIdx);
	if (rv != SCARD_S_SUCCESS)
		return rv;

	*data = NULL;
	BYTE * buffer = NULL;

	do {
		getrsp[ISO7816_OFFSET_LC] = 0x00;
		recvLen = 258;
		rv = MW_TransmitAPDU(getrsp, 5, pbRecvBuffer, &recvLen, cardSessionIdx);
		if (rv != SCARD_S_SUCCESS)
			return rv;

		getrsp[ISO7816_OFFSET_LC] = pbRecvBuffer[recvLen - 1];
		recvLen = 258;
		rv = MW_TransmitAPDU(getrsp, 5, pbRecvBuffer, &recvLen, cardSessionIdx);
		if (rv != SCARD_S_SUCCESS)
			return rv;


		if ((pbRecvBuffer[recvLen - 2] == 0x90) && (pbRecvBuffer[recvLen - 1] == 0x00))
			moreData = false;

		if (*data == NULL)
		{
			*data = (BYTE*)calloc(recvLen - 2, sizeof(BYTE));
			memcpy(*data, pbRecvBuffer, recvLen - 2);
			totalLen = recvLen - 2;
			continue;
		}

		buffer = (BYTE*)calloc(totalLen + recvLen - 2, sizeof(BYTE));
		memcpy(buffer, *data, totalLen);
		memcpy(buffer + totalLen, pbRecvBuffer, recvLen - 2);

		free(*data);
		*data = buffer;
		totalLen += recvLen - 2;


	} while (moreData);

	*dataLength = totalLen;

	memcpy(response->data, pbRecvBuffer, recvLen - 2);
	response->dataLen = recvLen - 2;
	response->status.SW1 = pbRecvBuffer[recvLen - 2];
	response->status.SW2 = pbRecvBuffer[recvLen - 1];

	return rv;
}
//End of MW_TransferDataGet function *******************************************************************************************************





//MW_GenerateSecretKey function *****************************************************************************************************************************
LONG MW_GenerateSecretKey(BYTE* label, DWORD labelSize, bool extractableFlag, BYTE keyType, DWORD keyLen, DWORD cardSessionIdx, APDURESPONSE *response) {
	LONG rv;

	BYTE generate[260];
	DWORD sendLen = 260;
	BYTE pbRecvBuffer[258];
	DWORD recvLen = 258;

	DWORD offset = ISO7816_OFFSET_CDATA;

	generate[ISO7816_OFFSET_CLA] = CLA;
	generate[ISO7816_OFFSET_INS] = INS_GENERATE_SECRET_KEY;
	generate[ISO7816_OFFSET_P1] = 0x00;
	generate[ISO7816_OFFSET_P2] = 0x00;
	generate[ISO7816_OFFSET_LC] = ((BYTE)(labelSize & 0xFF)) + 4;

	sendLen = 5 + generate[ISO7816_OFFSET_LC];

	generate[offset++] = (BYTE)(labelSize & 0xFF);
	memcpy(generate + offset, label, labelSize);
	offset += labelSize;

	generate[offset++] = extractableFlag ? (BYTE)0xFF : (BYTE)0x00;

	if (keyType == MW_SECRET_KEY_TYPE_AES)
	{
		generate[offset++] = 0x01;
		switch (keyLen) {
		case 128: generate[offset] = 0x80;
			break;
		case 192: generate[offset] = 0xC0;
			break;
		case 256: generate[offset] = 0x00;
			break;
		default: return SCARD_E_INVALID_PARAMETER;
		}
	}
	else if (keyType == MW_SECRET_KEY_TYPE_DES)
	{
		switch (keyLen) {
		case 64:  generate[offset++] = 0x02;
			generate[offset] = 0x40;
			break;
		case 128: generate[offset++] = 0x03;
			generate[offset] = 0x80;
			break;
		case 192: generate[offset++] = 0x03;
			generate[offset] = 0xC0;
			break;
		default: return SCARD_E_INVALID_PARAMETER;

		}
	}
	else return SCARD_E_INVALID_PARAMETER;


	rv = MW_TransmitAPDU(generate, sendLen, pbRecvBuffer, &recvLen, cardSessionIdx);
	if (rv == SCARD_W_RESET_CARD)
	{
		rv = SCardReconnect(scardCtx.sessions[cardSessionIdx].hCard, SCARD_SHARE_SHARED, scardCtx.sessions[cardSessionIdx].dwActiveProtocol, SCARD_RESET_CARD, &scardCtx.sessions[cardSessionIdx].dwActiveProtocol);
		if (rv != SCARD_S_SUCCESS)
			return rv;

		rv = MW_SelectApplet(cardSessionIdx, response);
		if (rv != SCARD_S_SUCCESS)
			return rv;

		rv = MW_TransmitAPDU(generate, sendLen, pbRecvBuffer, &recvLen, cardSessionIdx);
	}
	if (rv != SCARD_S_SUCCESS)
		return rv;

	if ((pbRecvBuffer[recvLen - 2] == 0x69) && (pbRecvBuffer[recvLen - 1] == 0x82))
		return SCARD_W_CARD_NOT_AUTHENTICATED;


	BYTE getrsp[] = { 0x00,0xC0,0x00,0x00,0x00 };

	sendLen = 5;
	recvLen = 258;
	rv = MW_TransmitAPDU(getrsp, sendLen, pbRecvBuffer, &recvLen, cardSessionIdx);
	if (rv != SCARD_S_SUCCESS)
		return rv;

	sendLen = 5;
	getrsp[ISO7816_OFFSET_LC] = pbRecvBuffer[recvLen - 1];
	recvLen = 258;
	rv = MW_TransmitAPDU(getrsp, sendLen, pbRecvBuffer, &recvLen, cardSessionIdx);
	if (rv != SCARD_S_SUCCESS)
		return rv;

	memcpy(response->data, pbRecvBuffer, recvLen - 2);
	response->dataLen = recvLen - 2;
	response->status.SW1 = pbRecvBuffer[recvLen - 2];
	response->status.SW2 = pbRecvBuffer[recvLen - 1];


	return rv;
}
//End of MW_GenerateSecretKey function **********************************************************************************************************************




*/

 byte GROUP_P[] = { (byte)0xD2, (byte)0x1A, (byte)0xE8, (byte)0xD6, (byte)0x6E, (byte)0x6C, (byte)0x6B, (byte)0x3C, (byte)0xED, (byte)0x0E,
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

 byte GROUP_Q[] = { 0x00,(byte)0xFF, (byte)0xF8, (byte)0x0A, (byte)0xE1, (byte)0x9D, (byte)0xAE, (byte)0xBC, (byte)0x61, (byte)0xF4, (byte)0x63,
(byte)0x56, (byte)0xAF, (byte)0x09, (byte)0x35, (byte)0xDC, (byte)0x0E, (byte)0x81, (byte)0x14, (byte)0x8E, (byte)0xB1 };

 byte GROUP_G[] = { 0x00,
(byte)0xAB, (byte)0xCE, (byte)0xC9, (byte)0x72, (byte)0xE9, (byte)0xA9, (byte)0xDD, (byte)0x8D, (byte)0x13, (byte)0x32,
(byte)0x70, (byte)0xCF, (byte)0xEA, (byte)0xC2, (byte)0x6F, (byte)0x72, (byte)0x6E, (byte)0x56, (byte)0x7D, (byte)0x96, (byte)0x47, (byte)0x57,
(byte)0x63, (byte)0x0D, (byte)0x6B, (byte)0xD4, (byte)0x34, (byte)0x60, (byte)0xD0, (byte)0x92, (byte)0x3A, (byte)0x46, (byte)0xAE, (byte)0xC0,
(byte)0xAC, (byte)0xE2, (byte)0x55, (byte)0xEB, (byte)0xF3, (byte)0xDD, (byte)0xD4, (byte)0xB1, (byte)0xC4, (byte)0x26, (byte)0x4F, (byte)0x53,
(byte)0xE6, (byte)0x8B, (byte)0x36, (byte)0x1A, (byte)0xFB, (byte)0x77, (byte)0x7A, (byte)0x13, (byte)0xCF, (byte)0x00, (byte)0x67, (byte)0xDA,
(byte)0xE3, (byte)0x64, (byte)0xA3, (byte)0x4D, (byte)0x55, (byte)0xA0, (byte)0x96, (byte)0x5A, (byte)0x6C, (byte)0xCC, (byte)0xF7, (byte)0x88,
(byte)0x52, (byte)0x78, (byte)0x29, (byte)0x23, (byte)0x81, (byte)0x3C, (byte)0xF8, (byte)0x70, (byte)0x88, (byte)0x34, (byte)0xD9, (byte)0x1F,
(byte)0x65, (byte)0x57, (byte)0xD7, (byte)0x83, (byte)0xEC, (byte)0x75, (byte)0xB5, (byte)0xF3, (byte)0x7C, (byte)0xD9, (byte)0x18, (byte)0x5F,
(byte)0x02, (byte)0x7B, (byte)0x04, (byte)0x2C, (byte)0x1C, (byte)0x72, (byte)0xE1, (byte)0x21, (byte)0xB1, (byte)0x26, (byte)0x6A, (byte)0x40,
(byte)0x8B, (byte)0xE0, (byte)0xBB, (byte)0x72, (byte)0x70, (byte)0xD6, (byte)0x59, (byte)0x17, (byte)0xB6, (byte)0x90, (byte)0x83, (byte)0x63,
(byte)0x3E, (byte)0x1F, (byte)0x3C, (byte)0xD6, (byte)0x06, (byte)0x24, (byte)0x61, (byte)0x2F, (byte)0xC8, (byte)0xC1 };

size_t UPROVE_BIG=128;
size_t UPROVE_SMALL = 20;

int main()
{
	
	byte selectAID[] = { 0x00,0xA4,0x04,0x00,0x8 ,0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0x01, 0x01, 0x7F };
	byte selectAIDPrimary[]  = { 0x00, 0xA4, 0x04, 0x00, 0xb, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x00, 0x00, 0x7F };
	short selectAIDlen = 14;

	Auth::IssuerParameters parameters;
	parameters.setP(GROUP_P, UPROVE_BIG);
	parameters.setQ(GROUP_Q, UPROVE_SMALL);
	parameters.setG(GROUP_G, UPROVE_BIG);

	Auth::Issuer issuer;
	issuer.publicIssuerInfo(parameters);



	/*
	CipherOffCard* offCard = CipherOffCard::getInstance(CipherOffCard::CIPHER_RECTANGLE);
	TestCase<CipherOffCard> *test = new CorelatedTestAttack < CipherOffCard, long>(2,10,10,0.1);
	test->setup(selectAID, 14);
	test->run(*offCard);
*/

	/*
	std::cout << "Media : " << std::setprecision(16) << test->getMedia() << std::endl;
	std::cout << "Dispersia : " << std::setprecision(10) << test->getSigma() << std::endl;
	std::cout << "Minim : " << std::setprecision(10) << test->getMinValue() << " Maxim :  ";
	std::cout << std::setprecision(10) << test->getMaxValue() << std::endl << std::endl;
	*/
	
	/*word sendLen = 14;
	Card c;
	c.connectToSlot();

	Apdu * req = new ApduSelection(selectAID, sendLen);
	Apdu & response = c.sendApdu(*req);
	StatusSW sw = response.getStatus();
	std::cout<<std::hex<<sw.toString();

	Apdu* apdu = new ApduRequest(0x00, 0x11, 0x23, 0x32, 0x0A, 0x00);
	byte* data = new byte[10]{ 0x00, 0x00 ,0x00, 0x00, 0x00, 0x00,0x00,0x00,0x00,0x00 };
	apdu->wrap(data, 10);
	response = c.sendApdu(*apdu);

	apdu = new ApduRequest(0x00,0x11,0x21,0x32,0x08,0x00);
	data = new byte[8]{0x00, 0x00 ,0x00, 0x00, 0x00, 0x00,0x00,0x00};
	apdu->wrap(data,8);
	response =  c.sendApdu(*apdu);
	std::cout << response.toString();
	*/

	// now encrypt
	/*
		Apdu* apdu = new ApduRequest(0x00, 0x11, 0x21, 0x32, 0x08, 0x00);
		byte* data = new byte[8]{ 0xff, 0xff ,0xff, 0xff, 0xff, 0xff,0xff,0xff };
		apdu->wrap(data, 8);
	
		long a = -1;
		TestCase<long>* test = new TestCase<long>(500,0.0000006);
		test->setup(selectAID,14);
		test->run(apdu);
		std::cout <<"Media : " <<  std::setprecision(16)<<test->getMedia()<<std::endl;
		std::cout <<"Dispersia : " << std::setprecision(10) <<test->getSigma() << std::endl;
		std::cout << "Minim : " << std::setprecision(10) << test->getMinValue() << " Maxim :  ";
		std::cout << std::setprecision(10) << test->getMaxValue() << std::endl << std::endl;
	*/

	system("pause");
	


}