#pragma once
#include "TestCase.h"
#include "CipherOffCard.h"
#include "Configuration.h"


template<typename Cipher,typename ExpecedValue>
class CorelatedTestAttack :
	public TestCase<Cipher>
{
	int noKeys;
	long noRecords;
public:
	virtual void run(Cipher & cipher, std::ostream& out);
	void runSecond(Cipher & cipher);
	CorelatedTestAttack(int noKeys,long noRecords, long noIterations,double epsilon);
	~CorelatedTestAttack();
};
template<typename Cipher, typename ExpecedValue>
void CorelatedTestAttack<Cipher, ExpecedValue>::run(Cipher & cipher, std::ostream& out = std::cout)
{
	if (cipher.getType() == 1)
	{
		runSecond(cipher);
		return;
	}
	Apdu * apdu;
	for (int i = 0; i < noKeys; i++)
	{
		apdu = cipher.getKey();
		Apdu& response = c.sendApdu(*apdu);
		StatusSW sw = response.getStatus();
		assert(sw.SW1() == 0x90 && sw.SW2() == 0x00);
		for (long  j = 0; j < noRecords; j++)
		{
			apdu = cipher.getBlock(cipher.MODE_ENCRYPT);
			double value = 0.0;
			int indexIteration = 0;
			maxValue = -1;
			minValue = 1000;
			for (int k = 0; k < noIterations; k++)
			{
				Apdu& response = c.sendApdu(*apdu);
				StatusSW sw = response.getStatus();
				assert(sw.SW1() == 0x90 && sw.SW2() == 0x00);
				vSigma[k] = response.getTimeElapsed();
				value += response.getTimeElapsed();
				if (response.getTimeElapsed() < minValue)
					minValue = response.getTimeElapsed();
				if (response.getTimeElapsed() > maxValue)
					maxValue = response.getTimeElapsed();
			}
			media = (double)(value / noIterations);
			sigma = 0;
			int noIterationSigma = noIterations;
			for (int k = 0; k < noIterations; k++)
			{

				double difference = this->vSigma[k] - media;
				double differenceIteration = difference / noIterations;

				if (abs(differenceIteration) > epsilon)
				{
					if (difference > 0)
						media -= differenceIteration;
					else
						media += differenceIteration;
					noIterationSigma--;
				}
				else
				{

					sigma += (vSigma[k] - media)*(vSigma[k] - media);
				}

			}
			sigma = (double)(sigma / noIterationSigma);
			sigma = sqrtf(sigma);
			std::cout << "media = " << media << " sigma = "<<sigma;
			std::cout << std::endl;
		}
	}
}
template<typename Cipher, typename ExpecedValue>
inline void CorelatedTestAttack<Cipher, ExpecedValue>::runSecond(Cipher & cipher)
{
	Apdu* apduKey;
	for (int i = 0; i < noKeys; i++)
	{
		apduKey = cipher.getKey();
		for (long j = 0; j < noRecords; j++)
		{
			Apdu * apdu = cipher.getBlock(cipher.MODE_ENCRYPT);
			apdu->update(apduKey);
			double value = 0.0;
			int indexIteration = 0;
			maxValue = -1;
			minValue = 1000;
			for (int k = 0; k < noIterations; k++)
			{
				Apdu& response = c.sendApdu(*apdu);
				StatusSW sw = response.getStatus();
				assert(sw.SW1() == 0x90 && sw.SW2() == 0x00);
				vSigma[k] = response.getTimeElapsed();
				value += response.getTimeElapsed();
				if (response.getTimeElapsed() < minValue)
					minValue = response.getTimeElapsed();
				if (response.getTimeElapsed() > maxValue)
					maxValue = response.getTimeElapsed();
			}
			media = (double)(value / noIterations);
			sigma = 0;
			int noIterationSigma = noIterations;
			for (int k = 0; k < noIterations; k++)
			{

				double difference = this->vSigma[k] - media;
				double differenceIteration = difference / noIterations;

				if (abs(differenceIteration) > epsilon)
				{
					if (difference > 0)
						media -= differenceIteration;
					else
						media += differenceIteration;
					noIterationSigma--;
				}
				else
				{

					sigma += (vSigma[k] - media)*(vSigma[k] - media);
				}

			}
			sigma = (double)(sigma / noIterationSigma);
			sigma = (double)sqrtf(sigma);
			std::cout << "media = " << media << " sigma = " << sigma;
			std::cout << std::endl;
		}
	}
}
template<typename Cipher, typename ExpecedValue>
CorelatedTestAttack<Cipher, ExpecedValue>::CorelatedTestAttack(int noKeys, long noRecords, long noIterations, double epsilon)
	:TestCase<Cipher>(noIterations, epsilon)
{
	this->noKeys = noKeys;
	this->noRecords = noRecords;
}
template<typename Cipher, typename ExpecedValue>
CorelatedTestAttack<Cipher, ExpecedValue>::~CorelatedTestAttack()
{

}
