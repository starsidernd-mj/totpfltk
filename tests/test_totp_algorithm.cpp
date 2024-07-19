#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include "../include/TOTP.h"

class MyTest : public CppUnit::TestCase {
    CPPUNIT_TEST_SUITE(MyTest);
    CPPUNIT_TEST(testBase32Decode);
    CPPUNIT_TEST(testSHA1);
    CPPUNIT_TEST_SUITE_END();

public:
	//Using the site below for confirming RFC6238 since original whitepaper uses invalid non-base 32 characters in Secret key
	//https://www.nayuki.io/page/time-based-one-time-password-tools
	
	std::string token = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
	
	std::string hello_enc = "JBCUYTCPEBLU6USMIQ======";
	std::string hello_dec = "HELLO WORLD";

	std::string test_enc = "KRSXG5DJNZTQ====";
	std::string test_dec = "Testing";

	std::string num_enc = "GEZDGNBVGY3TQOJQGEZDGNBVGY3TQOJQ";
	std::string num_dec = "12345678901234567890";

	std::string lorem_enc = "EAQCAICMN5ZGK3JANFYHG5LNEBSG63DPOIQHG2LUEBQW2ZLUFQQGG33OONSWG5DFOR2XEIDBMRUXA2LTMNUW4ZZAMVWGS5BMEBZWKZBAMRXSAZLJOVZW233EEB2GK3LQN5ZCA2LOMNUWI2LEOVXHIIDVOQQGYYLCN5ZGKIDFOQQGI33MN5ZGKIDNMFTW4YJAMFWGS4LVMEXCAVLUEBSW42LNEBQWIIDNNFXGS3JAOZSW42LBNUWCA4LVNFZSA3TPON2HE5LEEBSXQZLSMNUXIYLUNFXW4IDVNRWGC3LDN4QGYYLCN5ZGS4ZANZUXG2JAOV2CAYLMNFYXK2LQEBSXQIDFMEQGG33NNVXWI3ZAMNXW443FOF2WC5BOEBCHK2LTEBQXK5DFEBUXE5LSMUQGI33MN5ZCA2LOEBZGK4DSMVUGK3TEMVZGS5BANFXCA5TPNR2XA5DBORSSA5TFNRUXIIDFONZWKIDDNFWGY5LNEBSG63DPOJSSAZLVEBTHKZ3JMF2CA3TVNRWGCIDQMFZGSYLUOVZC4ICFPBRWK4DUMV2XEIDTNFXHIIDPMNRWCZLDMF2CAY3VOBUWIYLUMF2CA3TPNYQHA4TPNFSGK3TUFQQHG5LOOQQGS3RAMN2WY4DBEBYXK2JAN5TGM2LDNFQSAZDFONSXE5LOOQQG233MNRUXIIDBNZUW2IDJMQQGK43UEBWGCYTPOJ2W2LQ=";
	std::string lorem_dec = "    Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

	
	void testBase32Decode() {
		CPPUNIT_ASSERT_EQUAL(hello_dec, TOTP::base32_decode(hello_enc));
		CPPUNIT_ASSERT_EQUAL(test_dec, TOTP::base32_decode(test_enc));
		CPPUNIT_ASSERT_EQUAL(num_dec, TOTP::base32_decode(num_enc));
		CPPUNIT_ASSERT_EQUAL(lorem_dec, TOTP::base32_decode(lorem_enc));
	}
	
    void testSHA1() {
		/* times used
		0
		59
		1111111109
		1111111111
		1234567890
		2000000000
		20000000000
		*/
		//token
		CPPUNIT_ASSERT_EQUAL((std::string)"18081962", TOTP::generateTOTP(token, 0x0, 8));
		CPPUNIT_ASSERT_EQUAL((std::string)"62408553", TOTP::generateTOTP(token, 0x1, 8));
        CPPUNIT_ASSERT_EQUAL((std::string)"12594936", TOTP::generateTOTP(token, 0x23523ec, 8));
        CPPUNIT_ASSERT_EQUAL((std::string)"17966206", TOTP::generateTOTP(token, 0x23523ed, 8));
        CPPUNIT_ASSERT_EQUAL((std::string)"53111313", TOTP::generateTOTP(token, 0x273ef07, 8));
        CPPUNIT_ASSERT_EQUAL((std::string)"50660985", TOTP::generateTOTP(token, 0x3f940aa, 8));
        CPPUNIT_ASSERT_EQUAL((std::string)"47499790", TOTP::generateTOTP(token, 0x27bc86aa, 8));
		
		//hello_dec
		CPPUNIT_ASSERT_EQUAL((std::string)"65752758", TOTP::generateTOTP(hello_dec, 0x0, 8));
		CPPUNIT_ASSERT_EQUAL((std::string)"20186254", TOTP::generateTOTP(hello_dec, 0x1, 8));
        CPPUNIT_ASSERT_EQUAL((std::string)"80466734", TOTP::generateTOTP(hello_dec, 0x23523ec, 8));
        CPPUNIT_ASSERT_EQUAL((std::string)"77314839", TOTP::generateTOTP(hello_dec, 0x23523ed, 8));
        CPPUNIT_ASSERT_EQUAL((std::string)"25587334", TOTP::generateTOTP(hello_dec, 0x273ef07, 8));
        CPPUNIT_ASSERT_EQUAL((std::string)"16835429", TOTP::generateTOTP(hello_dec, 0x3f940aa, 8));
        CPPUNIT_ASSERT_EQUAL((std::string)"26284043", TOTP::generateTOTP(hello_dec, 0x27bc86aa, 8));
		
		//test_dec
		CPPUNIT_ASSERT_EQUAL((std::string)"75896222", TOTP::generateTOTP(test_dec, 0x0, 8));
		CPPUNIT_ASSERT_EQUAL((std::string)"48239813", TOTP::generateTOTP(test_dec, 0x1, 8));
        CPPUNIT_ASSERT_EQUAL((std::string)"87799118", TOTP::generateTOTP(test_dec, 0x23523ec, 8));
        CPPUNIT_ASSERT_EQUAL((std::string)"12720330", TOTP::generateTOTP(test_dec, 0x23523ed, 8));
        CPPUNIT_ASSERT_EQUAL((std::string)"38499979", TOTP::generateTOTP(test_dec, 0x273ef07, 8));
        CPPUNIT_ASSERT_EQUAL((std::string)"46965140", TOTP::generateTOTP(test_dec, 0x3f940aa, 8));
        CPPUNIT_ASSERT_EQUAL((std::string)"94328776", TOTP::generateTOTP(test_dec, 0x27bc86aa, 8));
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(MyTest);

int main(int argc, char** argv) {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());
    runner.run();
    return 0;
}

