#include <iostream>
#include <Polynomial.hpp>

using namespace std;

int main(void) 
{
    Polynomial p1{};
    p1.generatePolynomial(3,3,10);
    cout << "P1 = " << p1 << endl;


    Polynomial p2{};
    p2.generatePolynomial(3,5,5);
    cout << "P2 = " << p2 << endl;

    cout << "P1 + P2 = " << p1 + p2 << endl;
    cout << "P1 * P2 = " << p1 * p2 << endl;

    Polynomial p3{1};
    p3[0] = 1;
    p3[1] = 1;
    cout << p3 << endl;
    euclidianDiv(p1, p3);


    return 0;
}