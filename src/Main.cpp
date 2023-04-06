#include <iostream>
#include <tuple>

#include "Polynomial.hpp"
#include "SHE.hpp"

#include <gmpxx.h>

using namespace std;

void test_mpz(){
    mpz_class a{-45};
    int b = 2;
    cout << a << endl;
    cout << b << endl;
    cout << a+b << endl;
    cout << a*b << endl;
    cout << a/b << endl;
    cout << a%b << endl;
    cout << boolalpha << (a == b) << endl;

    mpz_class r;
    mpz_gcd(r.get_mpz_t(), a.get_mpz_t(), mpz_class{b}.get_mpz_t());
    cout << r << endl;
}

void test_polynomials(){
     cout << "==== Generation Polynome ====" << endl;
    Polynomial p1{1, {4000000, mpz_class{"2222222222222222222222222"}}};
    cout << "P1 = " << p1 << endl;

    Polynomial p2{1, {mpz_class{4000000}, mpz_class{"1111111111111111111111111"}}};
    cout << "P2 = " << p2 << endl;

    gmp_randstate_t state1;
    gmp_randinit_mt(state1);
    gmp_randseed_ui(state1, rand());
    Polynomial p3{10, mpz_class{"100"}, state1};
    cout << "P3 = " << p3 << endl;
    gmp_randclear(state1);

    cout << "==== Contenu ====" << endl;
    cout << "c(P1) = " << p1.contenu() << endl;

    cout << "==== Operations ====" << endl;
    cout << "P1 + P2 = " <<  p1 + p2 << endl;
    cout << "P1 - P1 = " << p1 - p1 << endl;
    cout << "P1 * P1 = " << p1 * p1 << endl;
    cout << "P1 / 2 = " << p1 / 2 << endl;
    cout << "P1 * 2 = " << p1 * mpz_class{2} << endl;

    cout << "==== Div Euclidienne ====" << endl;
    mpz_class d;
    Polynomial q,r;
    tie(d,q,r) = p1.EuclidianDiv(p2);
    cout << "d * p1 = " << p1 * d << endl;
    cout << "p2 * q + r = " <<  p2 * q + r << endl;   

    cout << "==== Bezout ====" << endl;
    gmp_randstate_t state2;
    gmp_randinit_mt(state2);
    gmp_randseed_ui(state2, rand());
    Polynomial p4{7, 100, state2};
    Polynomial p5{5, 100, state2};
    gmp_randclear(state2);
    cout << "P4 = " << p4 << endl;
    cout << "P5 = " << p5 << endl; 
    Polynomial R,U,V;
    tie(R,U,V) = p4.Bezout(p5);
    cout << "R = " << R << endl;
    cout << "P1 * U + P2 * V = " << (p4 * U) + (p5 * V) << endl;
}

void test_SHE(){
    SHE she{8};
    she.genKey();
    cout << she << endl;
}

int main(void) 
{
    srand(time(NULL));
    // test_mpz();
    // test_polynomials();
    test_SHE();
    return 0;
}