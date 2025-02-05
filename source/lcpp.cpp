// lcpp.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <chrono>
#include <ratio>
#include <ctime>
#include <numeric>
#include <cmath>
#include "sequence.h"
#include "Testmat1.h"
#include "TestBlas.h"

int main()
{
    try {
        int m = 10, n = 15, k = 10, q = 10; // q = 1000000;
        TestBlas blas;
        TestMatrix1 test1;
        //blas.test1(10000, q);
        //blas.test2(m,n,q);
        test1.testGEMM(m, n, k, q);
        test1.testTRSM();
        //test1.testTRMM();

    }
    catch (const std::exception& err) {
        std::cout << err.what();
    }
}

