#ifdef WIN32
#undef UNICODE
#endif

#include<winscard.h>
#include<vector>
#include "Benchmark.h"
#include "CipherOffCard.h"
#include "CorelatedTestAttack.h"
#include "SingleAutomaticTest.h"
#include "Issuer.h"
#include "TestInterface.h"
#include "SingleAutomaticTest.h"

 byte GROUP_Q[] = { 0x00,(byte)0xFF, (byte)0xF8, (byte)0x0A, (byte)0xE1, (byte)0x9D, (byte)0xAE, (byte)0xBC, (byte)0x61, (byte)0xF4, (byte)0x63,
(byte)0x56, (byte)0xAF, (byte)0x09, (byte)0x35, (byte)0xDC, (byte)0x0E, (byte)0x81, (byte)0x14, (byte)0x8E, (byte)0xB1 };

 byte GROUP_G[] = { 0x00,
(byte)0xAB, (byte)0xCE, (byte)0xC9, (byte)0x72, (byte)0xE9, (byte)0xA9, (byte)0xDD, (byte)0x8D, (byte)0x13, (byte)0x32,
(byte)0x70, (byte)0xCF, (byte)0xEA, (byte)0xC2, (byte)0x6F, (byte)0x72, (byte)0x6E, (byte)0x56, (byte)0x7D, (byte)0x96, (byte)0x47, (byte)0x57,
(byte)0x63, (byte)0x0D, (byte)0x6B, (byte)0xD4, (byte)0x34, (byte)0x60, (byte)0xD0, (byte)0x92, (byte)0x3A, (byte)0x46, (byte)0xAE, (byte)0xC0,
(byte)0xAC, (byte)0xE2, (byte)0x55, (byte)0xEB, (byte)0xF3, (byte)0xDD, (byte)0xD4, (byte)0xB1, (byte)0xC4, (byte)0x26, (byte)0x4F, (byte)0x53,
(byte)0xE6, (byte)0x8B, (byte)0x36, (byte)0x1A, (byte)0xFB, (byte)0x77, (byte)0x7A, (byte)0x13, (byte)0xCF, (byte)0x00, (byte)0x67, (byte)0xDA,
(byte)0xE3, (byte)0x64, (byte)0xA3, (byte)0x4D, (byte)0x55, (byte)0xA0, (byte)0x96, (byte)0x5A, (byte)0x6C, (byte)0xCC, (byte)0xF7, (byte)0x88,
(byte)0x52, (byte)0x78, (byte)0x29, (byte)0x23, (byte)0x81, (byte)0x3C, (byte)0xF8, (byte)0x70, (byte)0x88, (byte)0x34, (byte)0xD9, (byte)0x1F,
(byte)0x65, (byte)0x57, (byte)0xD7, (byte)0x83, (byte)0xEC, (byte)0x75, (byte)0xB5, (byte)0xF3, (byte)0x7C, (byte)0xD9, (byte)0x18, (byte)0x5F,
(byte)0x02, (byte)0x7B, (byte)0x04, (byte)0x2C, (byte)0x1C, (byte)0x72, (byte)0xE1, (byte)0x21, (byte)0xB1, (byte)0x26, (byte)0x6A, (byte)0x40,
(byte)0x8B, (byte)0xE0, (byte)0xBB, (byte)0x72, (byte)0x70, (byte)0xD6, (byte)0x59, (byte)0x17, (byte)0xB6, (byte)0x90, (byte)0x83, (byte)0x63,
(byte)0x3E, (byte)0x1F, (byte)0x3C, (byte)0xD6, (byte)0x06, (byte)0x24, (byte)0x61, (byte)0x2F, (byte)0xC8, (byte)0xC1 };
 size_t UPROVE_BIG	= 128;
 size_t UPROVE_SMALL = 20;


int main()
{
	try
	{
		#pragma region TEST_BIOMETRIC_MODULE
		try
		{
			SingleAutomaticTest<TestInterface> test;
			test.Test();
			std::cout << "Test passed";
		}
		catch (std::exception* exc)
		{
			std::cout << exc->what();
		}

#pragma endregion

		
		byte selectAID[] = { 0x00,0xA4,0x04,0x00,0x8 ,0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0x01, 0x01, 0x7F };
		byte selectAIDPrimary[]  = { 0x00, 0xA4, 0x04, 0x00, 0xb, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x00, 0x00, 0x7F };
		short selectAIDlen = 14;
		
		/*Auth::IssuerParameters parameters;
		parameters.setP(GROUP_P, UPROVE_BIG);
		parameters.setQ(GROUP_Q, UPROVE_SMALL);
		parameters.setG(GROUP_G, UPROVE_BIG);
		Auth::Issuer issuer;
		issuer.publicIssuerInfo(parameters);
		*/

		CipherOffCard* offCard = CipherOffCard::getInstance(CipherOffCard::CIPHER_RECTANGLE);
		TestCase<CipherOffCard> *test = new CorelatedTestAttack < CipherOffCard, long>(2,10,10,0.1);
		test->setup(selectAID, 14);
		test->run(*offCard);
		std::cout << "Media : " << std::setprecision(16) << test->getMedia() << std::endl;
		std::cout << "Dispersia : " << std::setprecision(10) << test->getSigma() << std::endl;
		std::cout << "Minim : " << std::setprecision(10) << test->getMinValue() << " Maxim :  ";
		std::cout << std::setprecision(10) << test->getMaxValue() << std::endl << std::endl;
		word sendLen = 14;
		Card c;
		c.connectToSlot();

		Apdu * req = new ApduSelection(selectAID, sendLen);
		Apdu & response = c.sendApdu(*req);
		StatusSW sw = response.getStatus();
		std::cout<<std::hex<<sw.toString();

		Apdu* apdu = new ApduRequest(0x00, 0x11, 0x23, 0x32, 0x0A, 0x00);
		byte* data = new byte[10]{ 0x00, 0x00 ,0x00, 0x00, 0x00, 0x00,0x00,0x00,0x00,0x00 };
		apdu->wrap(data, 10);
		response = c.sendApdu(*apdu);

		apdu = new ApduRequest(0x00,0x11,0x21,0x32,0x08,0x00);
		data = new byte[8]{0x00, 0x00 ,0x00, 0x00, 0x00, 0x00,0x00,0x00};
		apdu->wrap(data,8);
		response =  c.sendApdu(*apdu);
		std::cout << response.toString();
		Apdu* apdu = new ApduRequest(0x00, 0x11, 0x21, 0x32, 0x08, 0x00);
		byte* data = new byte[8]{ 0xff, 0xff ,0xff, 0xff, 0xff, 0xff,0xff,0xff };
		apdu->wrap(data, 8);

		long a = -1;
		TestCase<long>* test = new TestCase<long>(500,0.0000006);
		test->setup(selectAID,14);
		test->run(apdu);
		std::cout <<"Media : " <<  std::setprecision(16)<<test->getMedia()<<std::endl;
		std::cout <<"Dispersia : " << std::setprecision(10) <<test->getSigma() << std::endl;
		std::cout << "Minim : " << std::setprecision(10) << test->getMinValue() << " Maxim :  ";
		std::cout << std::setprecision(10) << test->getMaxValue() << std::endl << std::endl;
		system("pause");
	}
	
	catch (...)
	{

	}

}