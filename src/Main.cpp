#include <iostream>
#include <tuple>
#include "Polynomial.hpp"

using namespace std;

int main(void) 
{
    Polynomial p1{1, {BigInt{4000000}, BigInt{"2222222222222222222222222"}}};
    cout << "P1 = " << p1 << endl;
    cout << "c(P1) = ";
    p1.contenu().write(cout) << endl;

    Polynomial p2{1, {BigInt{4000000}, BigInt{"1111111111111111111111111"}}};
    cout << "P2 = " << p2 << endl;

    cout << "P1 + P2 = " <<  p1 + p2 << endl;
    cout << "P1 - P1 = " << p1 - p1 << endl;
    cout << "P1 * P1 = " << p1 * p1 << endl;

    cout << "P1 / 2 = " << p1 / BigInt{2} << endl;
    cout << "P1 * 2 = " << p1 * BigInt{2} << endl;

    BigInt d;
    Polynomial q,r;

    tie(d,q,r) = p1.EuclidianDiv(p2);
    d.write(cout) << " * (" << p1 << ") = (" << q << ") * (" << p2 << ") + " << r << endl;    

    return 0;
}