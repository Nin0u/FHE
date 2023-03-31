#include "Polynomial.hpp"
#include <iostream>

using namespace std;

int main(void) 
{
    Polynomial p1{};
    p1.generatePolynomial(5,5,10);
    cout << "P1 = " << p1 << endl;


    Polynomial p2{};
    p2.generatePolynomial(3,5,5);
    cout << "P2 = " << p2 << endl;

    cout << "P1 + P2 = " << p1 + p2 << endl;
    cout << "P1 * P2 = " << p1 * p2 << endl;

    Polynomial p3{4};
    p3[0] = 1;
    p3[4] = 2;
    cout << p3 << endl;
    euclidianDiv(p1, p3);
    //extendedGCD(p1, p3);


    return 0;
}