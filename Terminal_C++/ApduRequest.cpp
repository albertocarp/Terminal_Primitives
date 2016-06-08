#include "ApduRequest.h"





ApduRequest::ApduRequest(byte Cla, byte Ins, byte P1, byte P2, byte Lc, byte Le)
{
	this->Cla = Cla;
	this->Ins = Ins;
	this->P1 = P1;
	this->P2 = P2;
	this->Lc = Lc;
	this->Le = Le;
}

ApduRequest::ApduRequest(byte Cla, byte Ins, byte P1, byte P2, byte Lc)
	:ApduRequest(Cla,Ins,P1,P2,Lc,0)
{
}

ApduRequest::ApduRequest(byte Cla, byte Ins, byte P1, byte Lc)
	:ApduRequest(Cla,Ins,P1,0,Lc)
{
}

byte ApduRequest::getExpectedLen()const
{
	return Le;
}

bool ApduRequest::hasStatus()
{
	throw new SmartCardException("Invalid operation");
}

StatusSW ApduRequest::getStatus()
{
	throw new SmartCardException("Invalid operation");
}

void ApduRequest::wrap(byte * pData, word pLength)
{
	this->rawData = new byte[pLength + 6];
	this->rawData[Apdu::ISO7816_OFFSET_CLA] = Cla;
	this->rawData[Apdu::ISO7816_OFFSET_INS] = Ins;
	this->rawData[Apdu::ISO7816_OFFSET_P1]  = P1;
	this->rawData[Apdu::ISO7816_OFFSET_P2]  = P2;
	this->rawData[Apdu::ISO7816_OFFSET_LC]  = Lc;
	memcpy(this->rawData + 5, pData, pLength);
	this->rawData[pLength + 5] = Le;
	this->dataLength = pLength + 5;
}

void ApduRequest::wrap(vArray * data)
{
	this->dataLength = data->size() + 5;
	this->rawData = new byte[this->dataLength];

	this->rawData[Apdu::ISO7816_OFFSET_CLA] = Cla;
	this->rawData[Apdu::ISO7816_OFFSET_INS] = Ins;
	this->rawData[Apdu::ISO7816_OFFSET_P1] = P1;
	this->rawData[Apdu::ISO7816_OFFSET_P2] = P2;
	this->rawData[Apdu::ISO7816_OFFSET_LC] = Lc;

	int index = 5;
	for (auto  i = data->begin(); i != data->end(); i++)
	{
		this->rawData[index++] = (*i);
	}


}

void ApduRequest::update(Apdu * data)
{
	const byte * pData = data->getData();
	int length = data->getDataLength();

	word fullLength = this->getApduLength() + length;

	byte * newData = new byte[fullLength];
	newData[Apdu::ISO7816_OFFSET_CLA] = Cla;
	newData[Apdu::ISO7816_OFFSET_INS] = Ins;
	newData[Apdu::ISO7816_OFFSET_P1] = P1;
	newData[Apdu::ISO7816_OFFSET_P2] = P2;
	newData[Apdu::ISO7816_OFFSET_LC] = Lc + length;
	Lc += length;
	memcpy(newData+5, this->rawData+5, this->getDataLength());
	memcpy(newData+5+ this->getDataLength(), pData+5, length);
	this->rawData = new byte[fullLength];
	memcpy(this->rawData, newData, fullLength);
	this->dataLength = fullLength;
	//delete newData;

}

const word ApduRequest::getDataLength() const
{
	return Lc;
}

std::string ApduRequest::toString() const
{
	return std::string();
}

ApduRequest::~ApduRequest()
{
}
