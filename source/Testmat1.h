#ifndef __lcpp_testmat1_h
#define __lcpp_testmat1_h

class TestMatrix1{

public:

	TestMatrix1() {}

	void testGEMM(int m, int n, int k, int q);

	void testTRMM();

	void testTRSM();

};

#endif

