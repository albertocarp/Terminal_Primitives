#include "AplicationSpecificInfo.h"


using namespace Auth;

void Auth::AplicationSpecificInfo::setTokenInfoField(byte * TI, size_t TI_Size)
{
	this->TokenInfoField = new byte[TI_Size];
	memcpy(this->TokenInfoField, TI, TI_Size);
	this->TokenInfoFieldSize = TI_Size;
}

byte * Auth::AplicationSpecificInfo::getTokenInfoField()
{
	return TokenInfoField;
}

void Auth::AplicationSpecificInfo::addAttribute(byte * data, size_t size,size_t index)
{
	vArray* dataArray = new vArray();
	for (size_t i = 0; i < size; i++)
		dataArray->push_back(data[i]);
	if (this->Attributes.size() < index)
	{
		this->Attributes.push_back(*dataArray);
	}
	else
	{
		this->Attributes[index] = *dataArray;
	}
}

void Auth::AplicationSpecificInfo::setAttributesCount(int count)
{
	this->NumberAttributes = count;
}

size_t Auth::AplicationSpecificInfo::getAttributeCount()
{
	return NumberAttributes;
}

AplicationSpecificInfo::AplicationSpecificInfo()
{
}


AplicationSpecificInfo::~AplicationSpecificInfo()
{
}
