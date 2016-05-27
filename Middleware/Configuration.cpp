#include "Configuration.h"

byte* Configuration::selectAID;	
char Configuration::initialized;
void Configuration::init()
{
	selectAID = new byte[selectAidLen];
	byte selectAIDLocal[] = { 0x00,0xA4,0x04,0x00,0x8 ,0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0x01, 0x01, 0x7F };
	for (int i = 0; i < selectAidLen; i++)
	{
		selectAID[i] = selectAIDLocal[i];
	}
	initialized = 1;
}
