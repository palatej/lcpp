#include "Testmat1.h"
#include <iostream>
#include <chrono>
#include <ratio>
#include <ctime>
#include <numeric>
#include <cmath>

#include "trsm.h"

using namespace NUMCPP;
using namespace LCPP;

void
TestMatrix1::testTRSM() {
	int m = 5, k = 3;
	Matrix<double> A(m, m, [](int r, int c) { return (double)((r + 1) + 100 * (c + 1)); });
	Matrix<double> B(m, k, [](int r, int c) { return (double)(25 * (r + 1) - 25 * (c + 1)); });
	Matrix<double> Bt = transpose(B.all());
	TRSM<double> trsm;
	Matrix<double> Bc = B;
	trsm(Side::Left, Triangular::Lower, false, true, A.all(), 1, Bc.all());
	std::cout << A << std::endl << B << std::endl << Bc <<std::endl;
	Bc = B;
	trsm(Side::Left, Triangular::Upper, false, true, A.all(), 1, Bc.all());
	std::cout << Bc << std::endl;
	Bc = B;
	trsm(Side::Left, Triangular::Lower, false, false, A.all(), 1, Bc.all());
	std::cout << Bc << std::endl;
	Bc = B;
	trsm(Side::Left, Triangular::Upper, false, false, A.all(), 1, Bc.all());
	std::cout << Bc << std::endl;
	Bc = Bt;
	trsm(Side::Right, Triangular::Lower, false, true, A.all(), 1, Bc.all());
	std::cout << Bc << std::endl;
	Bc = Bt;
	trsm(Side::Right, Triangular::Upper, false, true, A.all(), 1, Bc.all());
	std::cout << Bc << std::endl;
	Bc = Bt;
	trsm(Side::Right, Triangular::Lower, false, false, A.all(), 1, Bc.all());
	std::cout << Bc << std::endl;
	Bc = Bt;
	trsm(Side::Right, Triangular::Upper, false, false, A.all(), 1, Bc.all());
	std::cout << Bc << std::endl;
	Bc = B;
	trsm(Side::Left, Triangular::Lower, true, true, A.all(), 1, Bc.all());
	std::cout << A << std::endl << B << std::endl << Bc << std::endl;
	Bc = B;
	trsm(Side::Left, Triangular::Upper, true, true, A.all(), 1, Bc.all());
	std::cout << Bc << std::endl;
	Bc = B;
	trsm(Side::Left, Triangular::Lower, true, false, A.all(), 1, Bc.all());
	std::cout << Bc << std::endl;
	Bc = B;
	trsm(Side::Left, Triangular::Upper, true, false, A.all(), 1, Bc.all());
	std::cout << Bc << std::endl;
	Bc = Bt;
	trsm(Side::Right, Triangular::Lower, true, true, A.all(), 1, Bc.all());
	std::cout << Bc << std::endl;
	Bc = Bt;
	trsm(Side::Right, Triangular::Upper, true, true, A.all(), 1, Bc.all());
	std::cout << Bc << std::endl;
	Bc = Bt;
	trsm(Side::Right, Triangular::Lower, true, false, A.all(), 1, Bc.all());
	std::cout << Bc << std::endl;
	Bc = Bt;
	trsm(Side::Right, Triangular::Upper, true, false, A.all(), 1, Bc.all());
	std::cout << Bc << std::endl;

}

