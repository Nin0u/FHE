#include <iostream>
#include <tuple>

#include "Polynomial.hpp"
#include "BigInt.hpp"

using namespace std;

// void test_bigint(){
//     BigInt bi1 = big_random(30);
//     cout << bi1 << endl;
// }

void test_polynomials(){
     cout << "==== Generation Polynome ====" << endl;
    Polynomial p1{1, {BigInt{4000000}, BigInt{"2222222222222222222222222"}}};
    cout << "P1 = " << p1 << endl;

    Polynomial p2{1, {BigInt{4000000}, BigInt{"1111111111111111111111111"}}};
    cout << "P2 = " << p2 << endl;

    Polynomial p3{10, BigInt{"123456789123456789"}};
    cout << "P3 = " << p3 << endl;

    cout << "==== Contenu ====" << endl;
    cout << "c(P1) = ";
    cout << p1 << endl;

    cout << "==== Operations ====" << endl;
    cout << "P1 + P2 = " <<  p1 + p2 << endl;
    cout << "P1 - P1 = " << p1 - p1 << endl;
    cout << "P1 * P1 = " << p1 * p1 << endl;
    cout << "P1 / 2 = " << p1 / BigInt{2} << endl;
    cout << "P1 * 2 = " << p1 * BigInt{2} << endl;

    cout << "==== Div Euclidienne ====" << endl;
    BigInt d;
    Polynomial q,r;
    tie(d,q,r) = p1.EuclidianDiv(p2);
    cout << "d * p1 = " << p1 * d << endl;
    cout << "p2 * q + r = " <<  p2 * q + r << endl;   

    cout << "==== Bezout ====" << endl;
    Polynomial R,U,V;
    tie(R,U,V) = p1.Bezout(p2);
    cout << "R = " << R << endl;
    cout << "P1 * U + P2 * V = " << (p1 * U) + (p2 * V) << endl;
}

// void test_SHE(){
//     cout << "ok" << endl;
//     SHE she{8};
//     she.genKey();
//     cout << she << endl;
// }

int main(void) 
{
    test_polynomials();
    //test_bigint();
    return 0;
}