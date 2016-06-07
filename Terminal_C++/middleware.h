#pragma once
#include<vector>

/*PKCS#15 applet CLA bytes*/
#define CLA           0x00
#define CLA_CMD_CHAIN 0x10


/*PKCS#15 applet INS bytes*/
#define INS_VERIFY						   0x20
#define INS_GET_RESPONSE				   0xC0
#define INS_TRANSFER_DATA_PUT			   0x02
#define INS_TRANSFER_DATA_GET			   0x04
#define INS_SETUP						   0x06
#define INS_GET_RANDOM_DATA				   0x07
#define INS_GENERATE_SECRET_KEY			   0x08
#define INS_GENERATE_KEY_PAIR			   0x09
#define INS_SYMMETRIC_ECB_ENCRYPT_DECRYPT  0x0A
#define INS_ASYMMETRIC_RSA_ENCRYPT_DECRYPT 0x0B
#define INS_COMPUTE_SIGNATURE			   0x0C
#define INS_IMPORT_SECRET_KEY			   0x0D
#define INS_EXPORT_SECRET_KEY			   0x0E
#define INS_EXPORT_PRIVATE_PUBLIC_KEY      0x0F
#define INS_IMPORT_PRIVATE_PUBLIC_KEY      0x01
#define INS_IMPORT_CERTIFICATE             0x03
#define INS_EXPORT_CERTIFICATE			   0x05
#define INS_UNBLOCK_AND_CHANGE_OWNER_PIN   0x10
#define INS_DELETE_OBJECT				   0x11
#define INS_LOGOUT						   0x12
#define INS_FIND_OBJECTS				   0x13


/*ISO7816 APDU offset values*/
#define ISO7816_OFFSET_CLA   0
#define ISO7816_OFFSET_INS   1
#define ISO7816_OFFSET_P1    2
#define ISO7816_OFFSET_P2    3
#define ISO7816_OFFSET_LC    4
#define ISO7816_OFFSET_CDATA 5


/*Middleware specific values*/
#define MW_USER_SO     0x01
#define MW_USER_NORMAL 0x00

#define MW_OBJECT_CLASS_PUBLIC_KEY  0x01
#define MW_OBJECT_CLASS_PRIVATE_KEY 0x02
#define MW_OBJECT_CLASS_SECRET_KEY  0x03
#define MW_OBJECT_CLASS_CERTIFICATE 0x04

#define MW_SECRET_KEY_TYPE_AES 0x02
#define MW_SECRET_KEY_TYPE_DES 0x01

#define MW_AES_KEY_LENGTH_128 128
#define MW_AES_KEY_LENGTH_192 192
#define MW_AES_KEY_LENGTH_256 256

#define MW_DES_KEY_LENGTH_64   64
#define MW_2DES_KEY_LENGTH_128 128
#define MW_3DES_KEY_LENGTH_192 192


#define MW_RSA_KEY_TYPE_PUBLIC  0x00
#define MW_RSA_KEY_TYPE_PRIVATE 0xFF

#define MW_RSA_KEY_LENGTH_1024 0x10
#define MW_RSA_KEY_LENGTH_2048 0x20

#define MW_CRYPTO_OPERATION_ENCRYPT 0x00
#define MW_CRYPTO_OPERATION_DECRYPT 0xFF

#define MW_ALG_DES 0x08
#define MW_ALG_AES 0x10

#define MW_SIGNATURE_ALG_RSA_SHA_PKCS1 0x00
#define MW_SIGNATURE_ALG_RSA_MD5_PKCS1 0xFF

#define MW_PADDING_NO_PAD 0x00
#define MW_PADDING_PKCS1 0xFF




typedef struct {
	SCARDHANDLE hCard;
	DWORD dwActiveProtocol;
	DWORD readerIdx;
} SCARDSESSION;


typedef struct {
	/*Winscard context*/
	SCARDCONTEXT context;
	/*Array with smart card readers names*/
	LPSTR* readers;
	/*boolean array indicating the presence of a token/smart card in associated reader*/
	bool *token_present_at;
	/*Number of smart card readers*/
	DWORD nrReaders;
	/*Vector with card sessions*/
	std::vector <SCARDSESSION> sessions;
}  SCARD_CTX;


/*APDU SW structure*/
typedef struct {
	BYTE SW1;
	BYTE SW2;
} STATUSW;

/*APDU response structure*/
typedef struct {
	BYTE data[256];
	DWORD dataLen;
	STATUSW status;
} APDURESPONSE;

extern SCARD_CTX scardCtx;
/**
	The connect function to the card
*/
extern LONG connect();

extern LONG releaseContext();

extern LONG MW_DetectReaders();

extern LONG MW_ConnectToSlot(DWORD readerIdx, DWORD* pCardSessionIdx);
extern LONG MW_DisconnectFromSlot(DWORD cardSessionIdx);

extern LONG MW_TransmitAPDU(BYTE* apdu, DWORD dwSendLength, BYTE* pbRecvBuffer, DWORD* pdwRecvLength, DWORD cardSessionIdx);

extern LONG MW_SelectApplet(DWORD cardSessionIdx, APDURESPONSE *response);

extern LONG MW_DoSetup(BYTE* soPIN, DWORD cardSessionIdx, APDURESPONSE* response);

extern LONG MW_Login(BYTE* pin, DWORD userType, DWORD cardSessionIdx, APDURESPONSE* response);
extern LONG MW_Logout(DWORD userType, DWORD cardSessionIdx, APDURESPONSE* response);

extern LONG MW_UnlockAndChangeOwnerPIN(BYTE* newPIN, DWORD cardSessionIdx, APDURESPONSE* response);

extern LONG MW_GetRandomData(BYTE bytesNr, DWORD cardSessionIdx, APDURESPONSE* response);

extern LONG MW_TransferDataPut(BYTE* data, DWORD dataLength, DWORD cardSessionIdx, APDURESPONSE* response);
extern LONG MW_TransferDataGet(BYTE** data, DWORD *dataLength, DWORD cardSessionIdx, APDURESPONSE* response);

extern LONG MW_GenerateSecretKey(BYTE* label, DWORD labelSize, bool extractableFlag, BYTE keyType, DWORD keyLen, DWORD cardSessionIdx, APDURESPONSE *response);
#pragma once
