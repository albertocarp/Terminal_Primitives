#include "Issuer.h"

void Auth::Issuer::publicIssuerInfo(Auth::IssuerParameters parameters)
{
	this->Parameters = parameters;
	// send apdu's --  send PQG
	Apdu* apdu = new ApduRequest(Configuration::APPLICATION_CLA, 
		Configuration::UPROVE_IDENTIFIER, Auth::SET_PQG, Auth::OFFET_P,(byte)this->Parameters.getPSize(),0x00);
	apdu->wrap(this->Parameters.getP(),this->Parameters.getPSize());
	Apdu& response = this->card.sendApdu(*apdu);
	response.getStatus().validate();

	apdu = new ApduRequest(Configuration::APPLICATION_CLA,
		Configuration::UPROVE_IDENTIFIER, Auth::SET_PQG, Auth::OFFET_Q, (byte)this->Parameters.getQSize(), 0x00);
	apdu->wrap(this->Parameters.getQ(), this->Parameters.getQSize());
	response = this->card.sendApdu(*apdu);
	response.getStatus().validate();

	apdu = new ApduRequest(Configuration::APPLICATION_CLA,
		Configuration::UPROVE_IDENTIFIER, Auth::SET_PQG, Auth::OFFET_G, (byte)this->Parameters.getGSize(), 0x00);
	apdu->wrap(this->Parameters.getG(), this->Parameters.getGSize());
	response = this->card.sendApdu(*apdu);
	response.getStatus().validate();
}

Auth::Issuer::Issuer(const Card & card, byte * selectedId, size_t selectdId_Size)
{
	this->card = card;
	this->card.connectToSlot();
	Apdu * req = new ApduSelection(selectedId, selectdId_Size);
	Apdu & response = this->card.sendApdu(*req);
	StatusSW sw = response.getStatus();
	sw.validate();
}

Auth::Issuer::Issuer()
{
}


Auth::Issuer::~Issuer()
{

}