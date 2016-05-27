#pragma once
#include "Card.h"
#include "TestCaseException.h"
template <typename ExpectedValue=value>
class TestCase
{
protected:
	int noIterations;
	double media;
	double sigma;
	Card c;
	double * vSigma;
	double * vValues;
	double minValue;
	double maxValue;
	double epsilon;
public:
	void assert(bool condition);
    void setup(byte* selectAID, short sendLen);
	virtual void run(ExpectedValue&,std::ostream& out = std::cout);
	virtual void run(Apdu*);
	double getMedia();
	double getSigma();
	double getMinValue();
	double getMaxValue();
	TestCase(int noIterations,double epsilon);
	~TestCase();
};


template<typename ExpectedValue>
void TestCase<ExpectedValue>::assert(bool condition)
{
	if (condition == false)
		throw new TestCaseException("Condition not verified.Test case failed");
}
template<typename ExpectedValue>
void TestCase<ExpectedValue>::setup(byte* selectAID,short sendLen)
{
	c.connectToSlot();
	Apdu * req = new ApduSelection(selectAID, sendLen);
	Apdu & response = c.sendApdu(*req);
	StatusSW sw = response.getStatus();
	this->assert(sw.SW1() == 0x90 && sw.SW2() == 0x00);
}

template<typename ExpectedValue>
void TestCase<ExpectedValue>::run(ExpectedValue&, std::ostream& out)
{
	double value=0.0;
	int indexIteration = 0;
	maxValue = -1;
	minValue = 1000;
	for (int i = 0; i < this->noIterations; i++)
	{
		Apdu* apdu = new ApduRequest(0x00, 0x01, 0x00, 0x00, 0x00, 0x00);
		apdu->wrap(nullptr, 0);
		Apdu & response = c.sendApdu(*apdu);
		StatusSW sw = response.getStatus();
		this->assert(sw.SW1() == 0x90 && sw.SW2() == 0x00);
		vSigma[i]  = response.getTimeElapsed();
		value	  += response.getTimeElapsed();
		if (response.getTimeElapsed() < minValue)
			minValue =  response.getTimeElapsed();
		if (response.getTimeElapsed() > maxValue)
			maxValue = response.getTimeElapsed();

	}
	media = (double)(value / noIterations);
	sigma = 0;
	int noIterationSigma = noIterations;
	for (int i = 0; i < noIterations;i++)
	{

		double difference = this->vSigma[i] - media;
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
			
			sigma += (vSigma[i] - media)*(vSigma[i] - media);
		}
		
	}
	sigma = (double)(sigma / noIterationSigma);
	sigma = sqrtf(sigma);
	std::cout << " It = " << noIterationSigma;


}
template<typename ExpectedValue>
inline void TestCase<ExpectedValue>::run(Apdu* apdu)
{
	double value = 0.0;
	int indexIteration = 0;
	maxValue = -1;
	minValue = 1000;
	for (int i = 0; i < this->noIterations; i++)
	{
		Apdu & response = c.sendApdu(*apdu);
		StatusSW sw = response.getStatus();
		this->assert(sw.SW1() == 0x90 && sw.SW2() == 0x00);
		vSigma[i] = response.getTimeElapsed();
		value += response.getTimeElapsed();
		if (response.getTimeElapsed() < minValue)
			minValue = response.getTimeElapsed();
		if (response.getTimeElapsed() > maxValue)
			maxValue = response.getTimeElapsed();

	}
	media = (double)(value / noIterations);
	sigma = 0;
	int noIterationSigma = noIterations;
	for (int i = 0; i < noIterations; i++)
	{

		double difference = this->vSigma[i] - media;
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

			sigma += (vSigma[i] - media)*(vSigma[i] - media);
		}

	}
	sigma = (double)(sigma / noIterationSigma);
	sigma = sqrtf(sigma);
	std::cout << " It = " << noIterationSigma;
}
template<typename ExpectedValue>
inline double TestCase<ExpectedValue>::getMedia()
{
	return media;
}
template<typename ExpectedValue>
inline double TestCase<ExpectedValue>::getSigma()
{
	return sigma;
}
template<typename ExpectedValue>
inline double TestCase<ExpectedValue>::getMinValue()
{
	return minValue;
}
template<typename ExpectedValue>
inline double TestCase<ExpectedValue>::getMaxValue()
{
	return maxValue;
}
template<typename ExpectedValue = value>
TestCase<ExpectedValue>::TestCase(int noIterations,double epsilon)
{
	this->noIterations = noIterations;
	vSigma = new double[this->noIterations];
	vValues = new double[this->noIterations];
	this->epsilon = epsilon;
	//setup();
}
template<typename ExpectedValue = value>
TestCase<ExpectedValue>::~TestCase()
{
	if(this->vSigma)
		delete this->vSigma;
	if(this->vValues)
		delete this->vValues;
}
