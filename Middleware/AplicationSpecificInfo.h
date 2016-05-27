#pragma once
#include "defs.h"
namespace Auth
{
	final SET_ATTRCOUNT = 0x49;
	final GET_ATTRCOUNT = 0x60;

	final SET_ATTRVAL = 0x62;
	final GET_ATTRVAL = 0x61;

	final SET_TI = 0x65;
	final GET_TI = 0x66;



	class AplicationSpecificInfo
	{
		std::vector<vArray> Attributes;
		int NumberAttributes;
		byte* TokenInfoField;
		size_t TokenInfoFieldSize;
	public:
		void setTokenInfoField(byte* TI, size_t TI_Size);
		byte* getTokenInfoField();
		void addAttribute(byte* data, size_t size,int index);
		void setAttributesCount(int count);
		size_t getAttributeCount();
		AplicationSpecificInfo();
		~AplicationSpecificInfo();
	};
}

