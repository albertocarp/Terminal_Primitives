#pragma once
#include "defs.h"

/*
#define for(size,body)\
	 for(int i = 0;i<size;i++){\
			body\
		}\

#define bodyValidate(val1,val2)\
			if(val1 != val2)\
			{\
				return false;\
			}

#define Test(func,type,expectedValue,arrayValue,sizeArray)\
	{\
		type data = func();\
		for(sizeArray)\
			 bodyValidate(arrayValue[i],expectedValue[i])\
		return true;\
	}		
	*/
namespace Auth
{
	static int NUMBER_ATTRIBUTE = 5;
	class IssuerParameters
	{
		byte * UidP;
		size_t UidP_Size;

		byte * UidH;
		size_t UidH_Size;

		byte * Specification;
		size_t Specification_Size;

		struct FieldDescription
		{
			byte* P;
			size_t P_Size;
			byte * Q;
			size_t Q_Size;
			byte * G;
			size_t G_Size;
		}FieldOrderPrime;
		std::vector<vArray>* issuerPubKeys;

	public:
		
		byte* getG();
		byte* getQ();
		byte* getP();
		byte* getUidP();
		byte* getUidH();
		byte* getSpecification();
		void setP(byte* P, size_t P_Size);
		void setQ(byte* Q, size_t Q_Size);
		void setG(byte* G, size_t G_Size);
		void setUidP(byte* UidP, size_t UidP_Size);
		void setUidH(byte* UidH, size_t UidH_Size);
		void setSpecification(byte* Specification, size_t Specification_Size);
		size_t getPSize();
		size_t getQSize();
		size_t getGSize();
		IssuerParameters();
		~IssuerParameters();
	};
}

