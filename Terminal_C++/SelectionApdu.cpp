#include "SelectionApdu.h"


ApduSelection::ApduSelection(byte * selectAID, word selectLength)
{
	rawData = new byte[selectLength + 1];
	memcpy(rawData, selectAID, selectLength);
	this->dataLength = selectLength;

}

ApduSelection::~ApduSelection()
{
}

bool ApduSelection::hasStatus()
{
	return true;
}

StatusSW ApduSelection::getStatus()
{
	return *this->status;
}

byte ApduSelection::getExpectedLen()const
{
	return (byte)258;
}

void ApduSelection::wrap(byte * pData, word pLength)
{
	//TODO fix this
}

void ApduSelection::wrap(vArray * data)
{
}

std::string ApduSelection::toString() const
{
	return std::string();
}
