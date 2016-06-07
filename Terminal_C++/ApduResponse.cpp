#include "ApduResponse.h"



ApduResponse::ApduResponse(byte*data,byte datalen)
{
	this->rawData = new byte[datalen];
	memcpy(this->rawData, data, datalen - 2);
	this->dataLength = datalen - 2;
	this->status = new StatusSW(data[datalen - 2], data[datalen - 1]);
}

ApduResponse::ApduResponse(byte * data, byte datalen, double time_elapsed)
{
	this->rawData = new byte[datalen];
	memcpy(this->rawData, data, datalen - 2);
	this->dataLength = datalen - 2;
	this->status = new StatusSW(data[datalen - 2], data[datalen - 1]);
	this->time_elpsed = time_elapsed;

}


ApduResponse::~ApduResponse()
{
}



byte ApduResponse::getExpectedLen()const
{
	return this->dataLength;
}

std::string  ApduResponse::toString()const
{
	std::stringstream stream;
	if (this->dataLength > 0)
	{
		stream << "\nData : ";
		for ( size_t i = 0 ; i < dataLength;i++ )
			stream<<std::uppercase<< std::hex<<(int)this->rawData[i]<<" ";
	}
	
	std::string statusData = status->toString();
	stream << statusData;
	stream << std::endl;
	stream << "Time : " << std::setprecision(16)<<this->time_elpsed;
	stream << std::endl;

	return stream.str();
}

bool ApduResponse::hasStatus()
{
	return true;
}

StatusSW ApduResponse::getStatus()
{
	return *this->status;
}

void ApduResponse::wrap(byte * pData, word pLength)
{

}

void ApduResponse::wrap(vArray * data)
{
}
