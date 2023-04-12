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
    SHE she{8, 1 << 30};
    she.genKey();
    cout << she << endl;

    cout << "==== Encrypt / Decrypt un bit ====" << endl;
    cout << "-- 0 --" << endl; 
    mpz_class e0 = she.encrypt(0);
    cout << "e0 = " << e0 << endl;
    mpz_class d0 = she.decrypt(e0);
    cout << "d(e0) = " << d0 << endl; 

    cout << "-- 1 --" << endl;
    mpz_class e1 = she.encrypt(1);
    cout << "e1 = " << e1 << endl;
    mpz_class d1 = she.decrypt(e1);
    cout << "d(e1) = " << d1 << endl; 

    cout << "==== Somme de deux chiffrés ====" << endl;
    cout << "-- 0 + 0 --" << endl;
    mpz_class c1 = she.encrypt(0);
    mpz_class c0 = she.encrypt(0);
    mpz_class s = c1 + c0;
    mpz_class d = she.decrypt(s);
    cout << "d(c1 + c0) = " << d <<  endl;

    cout << "-- 0 + 1 --" << endl;
    c1 = she.encrypt(0);
    c0 = she.encrypt(1);
    s = c1 + c0;
    d = she.decrypt(s);
    cout << "d(c1 + c0) = " << d <<  endl;

    cout << "-- 1 + 0 --" << endl;
    c1 = she.encrypt(1);
    c0 = she.encrypt(0);
    s = c1 + c0;
    d = she.decrypt(s);
    cout << "d(c1 + c0) = " << d <<  endl;

    cout << "-- 1 + 1 --" << endl;
    c1 = she.encrypt(1);
    c0 = she.encrypt(1);
    s = c1 + c0;
    d = she.decrypt(s);
    cout << "d(c1 + c0) = " << d <<  endl;


    cout << "==== Produit de deux chiffrés ====" << endl;
    cout << "-- 0 * 0 --" << endl;
    c1 = she.encrypt(0);
    c0 = she.encrypt(0);
    s = c1 * c0;
    d = she.decrypt(s);
    cout << "d(c1 * c0) = " << d << endl;

    cout << "-- 0 * 1 --" << endl;
    c1 = she.encrypt(0);
    c0 = she.encrypt(1);
    s = c1 * c0;
    d = she.decrypt(s);
    cout << "d(c1 * c0) = " << d << endl;


    cout << "-- 1 * 0 --" << endl;
    c1 = she.encrypt(1);
    c0 = she.encrypt(0);
    s = c1 * c0;
    d = she.decrypt(s);
    cout << "d(c1 * c0) = " << d << endl;

    cout << "-- 1 * 1 --" << endl;
    c1 = she.encrypt(1);
    c0 = she.encrypt(1);
    s = c1 * c0;
    d = she.decrypt(s);
    cout << "d(c1 * c0) = " << d << endl;

    cout << "==== Test avec Polynome ====" << endl;
    for(int i = 0; i < 100; i++) {
        bool b;
        if (!(b = she.testPolynomial(2 + i, 0))) break;
        else cout << boolalpha << b << endl;

        if (!(b = she.testPolynomial(2 + i, 1))) break;
        else cout << boolalpha << b << endl;
    }
}

void test_SHEM()
{
    int deg = 3;
    SHE she{deg, 100};
    she.genKey();
    cout << she << endl;

    cout << "==== Encrypt / Decrypt plusieurs bits ====" << endl;
    vector<char> bits{};
    //bits.resize(1 << deg);
    cout << "bits = ";
    for(int i = 0; i < 1 << deg; i++)
        bits.push_back(rand() % 2);
    for(int i = 0; i < 1 << deg; i++)
        printf("%d ", bits[i]);
    cout << endl;

    mpz_class c = she.encryptM(bits);
    cout << "c = " << c << endl;

    cout << "bits = ";
    vector<mpz_class> dec = she.decryptM(c);
    for(int i = 0; i < 1 << deg; i++)
        cout << dec[i] << " ";
    cout << endl;


    cout << "==== XOR ====" << endl;
    vector<char> bit1s{};
    //bits.resize(1 << deg);
    cout << "bit1s = ";
    for(int i = 0; i < 1 << deg; i++)
        bit1s.push_back(rand() % 2);
    for(int i = 0; i < 1 << deg; i++)
        printf("%d ", bit1s[i]);
    cout << endl;

    vector<char> bit2s{};
    //bits.resize(1 << deg);
    cout << "bit2s = ";
    for(int i = 0; i < 1 << deg; i++)
        bit2s.push_back(rand() % 2);
    for(int i = 0; i < 1 << deg; i++)
        printf("%d ", bit2s[i]);
    cout << endl;

    vector<char> bit3s{};
    for(int i = 0; i < 1 << deg; i++)
        bit3s.push_back(bit1s[i] ^ bit2s[i]);
    cout << "bit3s = ";
    for(int i = 0; i < 1 << deg; i++)
        printf("%d ", bit3s[i]);
    cout << endl;

    mpz_class c1 = she.encryptM(bit1s);
    mpz_class c2 = she.encryptM(bit2s);
    mpz_class c3 = she.addCipher(c1, c2);
    vector<mpz_class> bitds = she.decryptM(c3);
    cout << "bitds = ";
    for(int i = 0; i < 1 << deg; i++)
        cout << bitds[i] << " ";
    cout << endl;

    c1 = she.encryptM(bit1s);
    c2 = she.encryptM(bit2s);
    mpz_class c4 = she.mulCipher(c1, c2);
    vector<mpz_class> bitMs = she.decryptM(c4);
    // Le multiplication le fait modulo X^n + 1, donc X^n = -1 = 1 dans F2
    cout << "bitMs = ";
    for(int i = 0; i < 1 << deg; i++)
        cout << bitMs[i] << " ";
    cout << endl;
}

void test_squash()
{
    SHE she{4, 100};
    she.genKey();

    char b1 = 1;
    char b2 = 0;

    mpz_class c1 = she.encrypt(b1);
    mpz_class c2 = she.encrypt(b2);

    mpz_class C1 = she.encrypt(b1);
    mpz_class C2 = she.encrypt(b2);


    mpz_class c3 = she.addCipher(c1, c2);
    mpz_class c4 = she.mulCipher(c1, c2);
    mpz_class c5 = she.addCipher(c1, C1);
    mpz_class c6 = she.addCipher(c2, C2);
    mpz_class c7 = she.mulCipher(c1, C1);
    mpz_class c8 = she.mulCipher(c2, C2);

    cout << "1 + 0 = " << she.decrytpSquash(she.expandCT(c3)) << endl;
    cout << "1 * 0 = " << she.decrytpSquash(she.expandCT(c4)) << endl;
    cout << "1 + 1 = " << she.decrytpSquash(she.expandCT(c5)) << endl;
    cout << "0 + 0 = " << she.decrytpSquash(she.expandCT(c6)) << endl;
    cout << "1 * 1 = " << she.decrytpSquash(she.expandCT(c7)) << endl;
    cout << "0 * 0 = " << she.decrytpSquash(she.expandCT(c8)) << endl;

}

int main(void) 
{
    srand(time(NULL));
    // test_mpz();
    // test_polynomials();
    // test_SHE();
    // test_SHEM();
    test_squash();
    return 0;
}