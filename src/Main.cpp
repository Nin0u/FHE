#include <iostream>
#include <fstream>
#include <tuple>
#include <gmpxx.h>
#include <unistd.h>
#include <thread>
#include <time.h>

#include "Polynomial.hpp"
#include "SHE.hpp"
#include "Algo.hpp"
#include "Cipher.hpp"
#include "Gen.hpp"

using namespace std;

void print_bar(float progress)
{
    int barWidth = 37;
    cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) cout << "=";
        else if (i == pos) cout << ">";
        else cout << " ";
    }

    cout << "] " << int(progress * 100.0) << " %\r";
    cout.flush();
}


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
    she.genKey(1);
    cout << she << endl;

    cout << "==== Encrypt / Decrypt un bit ====" << endl;
    cout << "-- 0 --" << endl; 
    Cipher e0 = she.encrypt(0);
    cout << "e0 = " << e0 << endl;
    mpz_class d0 = she.decrypt(e0);
    cout << "d(e0) = " << d0 << endl; 

    cout << "-- 1 --" << endl;
    Cipher e1 = she.encrypt(1);
    cout << "e1 = " << e1 << endl;
    mpz_class d1 = she.decrypt(e1);
    cout << "d(e1) = " << d1 << endl; 

    cout << "==== Somme de deux chiffrés ====" << endl;
    cout << "-- 0 + 0 --" << endl;
    Cipher c1 = she.encrypt(0);
    Cipher c0 = she.encrypt(0);
    Cipher s = c1 + c0;
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
    int deg = 8;
    SHE she{deg, 100};
    she.genKey(1);
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

    Cipher c = she.encryptM(bits);
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

    Cipher c1 = she.encryptM(bit1s);
    Cipher c2 = she.encryptM(bit2s);
    Cipher c3 = c1 + c2;
    vector<mpz_class> bitds = she.decryptM(c3);
    cout << "bitds = ";
    for(int i = 0; i < 1 << deg; i++)
        cout << bitds[i] << " ";
    cout << endl;

    c1 = she.encryptM(bit1s);
    c2 = she.encryptM(bit2s);
    Cipher c4 = c1 * c2;
    vector<mpz_class> bitMs = she.decryptM(c4);
    // Le multiplication le fait modulo X^n + 1, donc X^n = -1 = 1 dans F2
    cout << "bitMs = ";
    for(int i = 0; i < 1 << deg; i++)
        cout << bitMs[i] << " ";
    cout << endl;
}

void test_squash()
{
    SHE she{6, 100};
    she.genKey(1);

    char b1 = 1;
    char b2 = 0;

    Cipher c1 = she.encrypt(b1);
    Cipher c2 = she.encrypt(b2);

    Cipher C1 = she.encrypt(b1);
    Cipher C2 = she.encrypt(b2);


    Cipher c3 = c1 + c2;
    Cipher c4 = c1 * c2;
    Cipher c5 = c1 + C1;
    Cipher c6 = c2 + C2;
    Cipher c7 = c1 * C1;
    Cipher c8 = c2 * C2;

    cout << "==== Normal Decrypt ====" << endl;
    cout << "1 + 0 = " << she.decrypt(c3) << endl;
    cout << "1 * 0 = " << she.decrypt(c4) << endl;
    cout << "1 + 1 = " << she.decrypt(c5) << endl;
    cout << "0 + 0 = " << she.decrypt(c6) << endl;
    cout << "1 * 1 = " << she.decrypt(c7) << endl;
    cout << "0 * 0 = " << she.decrypt(c8) << endl;
    cout << endl;


    cout << "==== First Squash ====" << endl;
    cout << "1 + 0 = " << she.decryptSquash(she.expandCT(c3)) << endl;
    cout << "1 * 0 = " << she.decryptSquash(she.expandCT(c4)) << endl;
    cout << "1 + 1 = " << she.decryptSquash(she.expandCT(c5)) << endl;
    cout << "0 + 0 = " << she.decryptSquash(she.expandCT(c6)) << endl;
    cout << "1 * 1 = " << she.decryptSquash(she.expandCT(c7)) << endl;
    cout << "0 * 0 = " << she.decryptSquash(she.expandCT(c8)) << endl;
    cout << endl;

    cout << "==== Real Squash ====" << endl;
    cout << 0 << " = " << she.decryptRealSquash(she.expandCT(c2)) << endl;
    cout << 1 << " = " << she.decryptRealSquash(she.expandCT(c1)) << endl;
    cout << "1 + 0 = " << she.decryptRealSquash(she.expandCT(c3)) << endl;
    cout << "1 * 0 = " << she.decryptRealSquash(she.expandCT(c4)) << endl;
    cout << "1 + 1 = " << she.decryptRealSquash(she.expandCT(c5)) << endl;
    cout << "0 + 0 = " << she.decryptRealSquash(she.expandCT(c6)) << endl;
    cout << "1 * 1 = " << she.decryptRealSquash(she.expandCT(c7)) << endl;
    cout << "0 * 0 = " << she.decryptRealSquash(she.expandCT(c8)) << endl;

}

void test_GSA()
{
    cout << "==== Next Set ====" << endl;
    vector<int> v{};
    v.push_back(0);
    v.push_back(1);

    for(int i : v)
        cout << i << " ";
    cout << endl;

    int nb = 5;
    int iter = 0;
    while(!(v[0] == 0 && v[1] == 0))
    {
        v = next_set(v, nb);

        for(int i : v)
            cout << i << " ";
        cout << endl;
        iter++;
    }
    cout << endl;


    cout << "==== GSA 1 ====" << endl;
    vector<mpz_class> v1 = {0,0,1,1,1,1};    
    vector<mpz_class> v2 = {1,1,0,0,1,1};    
    vector<mpz_class> v3 = {0,1,1,1,1,1};    
    vector<mpz_class> v4 = {1,0,0,1,0,1};    
    vector<mpz_class> v5 = {0,0,0,0,0,0};    
    vector<mpz_class> v6 = {0,0,0,0,0,0};
    
    vector<vector<mpz_class>> columns{};
    columns.push_back(v1);
    columns.push_back(v2);
    columns.push_back(v3);
    columns.push_back(v4);
    columns.push_back(v5);
    columns.push_back(v6);

    vector<mpz_class> r = gradeSchoolAddition(columns);
    for(mpz_class b : r) 
        cout << b << " ";
    cout << endl;

    cout << "==== GSA 2 ====" << endl;

    nb = 2;
    vector<mpz_class> vec{};
    int sum = 0;
    for(int i = 0; i < nb; i++) {
        int val = rand() % 256;
        //if((val & 1) == 0) val += 1;
        cout << val << " ";
        sum += val;
        vec.push_back(val);
    }
    cout << endl;

    vector<vector<mpz_class>> col{};
    int precision = 16;
    col.resize(precision);
    for(int i = 0; i < precision; i++)
        col[i].resize(nb);

    for(int j = 0; j < nb; j++) {
        for(int i = 0; i < precision; i++) {
            col[i][j] = (vec[j] >> i) & 1;
        }
    }

    for(int j = 0; j < nb; j++) {
        for(int i = precision - 1; i >= 0; i--) {
            cout << col[i][j] << " ";
        }
        cout << endl;
    }

    cout << "-------------------------------" << endl;

    vector<mpz_class> repon = gradeSchoolAddition(col);
    for(int i = precision - 1; i >= 0; i--) {
        cout << repon[i] << " ";
    }
    cout << endl;

    mpz_class fi = 0;
    for(int i = 0; i < precision; i++) {
        fi += repon[i] << i;
    }

    cout << sum << " " << fi << endl;
}

void test_recrypt()
{
    mpz_class max = 1;
    max <<= 380;
    SHE she{10, max};
    she.genKey(1);

    char b1 = 1;
    char b2 = 0;

    cout << "Clear / Real / Normal / Recrypt" << endl;
    for(int i = 0; i < 10; i++)
    {
        Cipher c1 = she.encrypt(b2);

        Cipher c3 = she.recrypt(c1);

        cout << "0 == " << she.decryptRealSquash(she.expandCT(c1))
        << " == " << she.decrypt(c1) << " == " << she.decryptRealSquash(she.expandCT(c3)) << endl;
    }
    cout << endl;
    for(int i = 0; i < 10; i++)
    {
        Cipher c1 = she.encrypt(b1);

        Cipher c3 = she.recrypt(c1);

        cout << "1 == " << she.decryptRealSquash(she.expandCT(c1))
        << " == " << she.decrypt(c1) << " == " << she.decryptRealSquash(she.expandCT(c3)) << endl;
    }

    cout << endl;

    Cipher c1 = she.encrypt(b1);
    Cipher c2 = she.encrypt(b2);

    Cipher c3 = she.recrypt(c1 + c1);
    Cipher c4 = she.recrypt(c2 + c2);
    Cipher c5 = she.recrypt(c1 + c2);
    Cipher c6 = she.recrypt(c1 * c2);
    Cipher c7 = she.recrypt(c1 * c1);
    Cipher c8 = she.recrypt(c2 * c2);
    cout << "1 + 1 = " << she.decryptRealSquash(she.expandCT(c3)) << endl;
    cout << "0 + 0 = " << she.decryptRealSquash(she.expandCT(c4)) << endl;
    cout << "1 + 0 = " << she.decryptRealSquash(she.expandCT(c5)) << endl;
    cout << "1 * 0 = " << she.decryptRealSquash(she.expandCT(c6)) << endl;
    cout << "1 * 1 = " << she.decryptRealSquash(she.expandCT(c7)) << endl;
    cout << "0 * 0 = " << she.decryptRealSquash(she.expandCT(c8)) << endl;
    cout << endl;

    cout << "Calul max degré : ";
    cout.flush();
    int degmax = 0;
    for(int i = 0; i < 1000; i++) {
        bool b;
        if (!(b = she.testPolynomial(2 + i, 0))) break;
        //else cout << boolalpha << b << endl;

        if (!(b = she.testPolynomial(2 + i, 1))) break;
        //else cout << boolalpha << b << endl;
        degmax++;
        if(i % 10 == 0) {
            cout << ". ";
            cout.flush();
        }
    }

    cout << endl;
    cout << "Degré Max : " << degmax << endl;
    cout << endl;
    
    Cipher cb = she.encrypt(b1);
    char res = 1;
    
    int max_iter = 1000;
    cout << "==== Test Eval Polynome Degré " << max_iter << " ====" << endl;
    bool success = true;
    for(int i = 0; i < max_iter; i++) {
        int r = rand() % 2;
        char bit = rand() % 2;
        Cipher c = she.encrypt(bit);
        if(r == 0) {
            res ^= bit;
            cb = cb + c;
        } else {
            res = res & bit;
            cb = cb * c;
        }
        
        cb = she.recrypt(cb);

        mpz_class dec = she.decrypt(cb);
        if(dec != res) {
            cout << endl;
            cout << "ERROR " << i << endl;
            success = false;
            break;
        } else {
            print_bar(i * 1.0 / max_iter);
        }
    }

    if(success) {
        print_bar(1.0);
        cout << endl;
        cout << "Success !" << endl;
    }
}


mpz_class perform_sum(int a, int b, int max_bits, SHE &she)
{
    vector<int> bits_a{};
    vector<int> bits_b{};

    bits_a.resize(max_bits);
    bits_b.resize(max_bits);

    for(int i = 0; i < max_bits; i++) {
        bits_a[i] = (a >> i) & 1;
        bits_b[i] = (b >> i) & 1;
    }

    vector<vector<Cipher>> columns{};
    columns.resize(max_bits);
    for(int i = 0; i < max_bits; i++) {
        columns[i].resize(2);
    }

    for(int j = 0; j < max_bits; j++) {
        columns[j][0] = she.encrypt(bits_a[j]);
        columns[j][1] = she.encrypt(bits_b[j]);
    }

    vector<Cipher> res = gradeSchoolAddition(columns);

    mpz_class resint = 0;

    for(int i = 0; i < max_bits; i++) {
        resint += she.decrypt(res[i]) << i;
    }

    return resint;
}

void test_sum_int() {
    mpz_class max = 1;
    max <<= 380;
    SHE she{6, max};
    she.genKey(0);

    int a = rand() % 256;
    int b = rand() % 256;

    cout << a << " " << b << endl;

    mpz_class sum = perform_sum(a, b, 16, she);
    cout << (a + b) << " == " << sum << endl;
}

void test_mod() {
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, rand());
    Polynomial p1{7, 100, state};
    Polynomial p2{5, 100, state};
    gmp_randclear(state);

    cout << "P1 = " << p1 << endl;
    cout << "P2 = " << p2 << endl;

    Polynomial q, r;
    mpz_class mod = 5;
    p1.reduce(mod);
    p2.reduce(mod);

    cout << endl;
    cout << "P1 = " << p1 << endl;
    cout << "P2 = " << p2 << endl;

    tie(q,r) = p1.EuclidianDiv(p2, mod);
    cout << "Q = " << q << endl;
    cout << "R = " << r << endl;
    Polynomial p3 = p2 * q + r;
    p3.reduce(mod);
    cout << "P3 = " << p3 << endl;


    Polynomial p4, p5, p6;
    tie(p4, p5, p6) = p1.Bezout(p2, mod);

    cout << "R = " << p4 << endl;
    Polynomial p7 = p5 * p1 + p6 * p2;
    p7.reduce(mod);

    cout << "P1 * U + P2 * V = " << p7 << endl;
}

void getint(int i, int *rep) {
    if(i == 0) {
        *rep = 1;
        return;
    }
    else {
        int a = *rep;
        int b = *rep;
        thread first(getint, i - 1, &a);
        thread second(getint, i - 1, &b);

        first.join();
        second.join();
        
        *rep = a + b;
        return; 
    }
} 

void test_thread()
{
    int rep = 0;
    getint(10, &rep);
    cout << rep << endl;
}

void test_invert_pol()
{
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, rand());
    
    cout << "==== TEST CRT ====" << endl;


    mpz_class big = 1;
    big <<= 50;
    cout << big << endl;
    mpz_class prime1, prime2;
    mpz_nextprime (prime1.get_mpz_t(), big.get_mpz_t());
    mpz_nextprime (prime2.get_mpz_t(), prime1.get_mpz_t());
    cout << prime1 << " " << prime2 << endl;

    mpz_class a, b;
    big << 50;
    mpz_urandomm(a.get_mpz_t(), state, big.get_mpz_t());
    mpz_urandomm(b.get_mpz_t(), state, big.get_mpz_t());

    cout << a << " " << b << endl;
    
    a %= prime1;
    b %= prime2;

    mpz_class c = CRT(a, prime1, b, prime2);
    if(c < 0) c += prime1 * prime2;
    if(a < 0) a += prime1;
    if(b < 0) b += prime2;
    cout << a << " == " << c % prime1 << " " << b << " == " << c % prime2 << endl; 


    cout << "==== TEST POLYNOME ====" << endl;
    int deg = 1 << 10;

    mpz_class max{1};
    max <<= 256;
    Polynomial p1{deg - 1, max, state};

    Polynomial p2{deg};
    p2[deg] = 1;
    p2[0] = 1;
    
    cout << "p1 : " << p1 << endl;
    cout << "p2 : " << p2 << endl;

    Polynomial w;
    mpz_class d;

    //! ATTENTION : le 3eme argument, sur votre PC, faite attention, pas audessus de 15, et au dessus de 10 peut faire planter le PC
    //! Correspond à 2^(3eme argument) threads !!!!!
    tie(w, d) = invert_Polynomial(p1, p2, 10, 100);

    //cout << "w = " << w << endl;
    cout << "d = " << d << endl;
    cout << "w1 = " << w[1] << endl; 

    // Polynomial q, r;
    // Polynomial p3 = p1 * w;
    // mpz_class aa;
    // tie(aa, q, r) = p3.EuclidianDiv(p2);
    // cout << r << endl;


    Polynomial G, U, V;
    tie(G, U, V) = p2.Bezout(p1);
    cout << "**d = " << G << endl;
    //cout << "**w = " << V << endl;
    cout << "**w0 = " << V[0] << endl;
    cout << "**w1 = " << V[0] << endl; 

    mpz_class r1 = d / G[0];
    mpz_class r2 = w[0] / V[0];
    mpz_class r3 = w[1] / V[1];
    cout << "r1 = " << r1 << endl;
    cout << "r2 = " << r2 << endl;
    cout << "r3 = " << r3 << endl;
    mpz_class gcd1;
    mpz_class gcd;
    mpz_gcd(gcd1.get_mpz_t(), d.get_mpz_t(), w[0].get_mpz_t());
    mpz_gcd(gcd.get_mpz_t(),gcd1.get_mpz_t(), w[1].get_mpz_t());
    cout << "gcd = " << gcd << endl;

    bool b1 = ((r1 * G[0]) == d);
    bool b2 = ((r2 * V[0]) == w[0]);
    bool b3 = ((r3 * V[1]) == w[1]);
    cout << boolalpha << b1 << endl;
    cout << boolalpha << b2 << endl;
    cout << boolalpha << b3 << endl;


    gmp_randclear(state);
}

void test_di()
{
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, rand());

    int de = 1 << 3;

    Polynomial p1{de - 1, 100, state};

    Polynomial p2{de};
    p2[de] = 1;
    p2[0] = 1;
    
    cout << "p1 : " << p1 << endl;
    cout << "p2 : " << p2 << endl;

    mpz_class a = {1};
    a <<= 60;

    int deg = 2 * p2.getDeg();

    mpz_class prime;
    do {
        mpz_nextprime (prime.get_mpz_t(), a.get_mpz_t());
        a = prime;
    } while(a % (deg) != 1);
    cout << "prime = 1 mod 2^" << deg << " : " << a << endl;

    mpz_class d, w0, w1;
    tie(d, w0, w1) = get_di(p1, prime, p2.getDeg());

    cout << "di : " << d << endl;
    cout << "w0 : " << w0 << endl;
    cout << "w1 : " << w1 << endl;

    Polynomial G, U, V;
    tie(G, U, V) = p2.Bezout(p1);
    cout << "**d = " << G << endl;
    cout << "**di = " << G[0] % prime << endl;
    cout << "**w0 = " << V[0] % prime << endl;  
    cout << "**w1 = " << V[1] % prime << endl;

    gmp_randclear(state);
}

void test_deg_recrypt()
{
    int deg = 6;
    mpz_class max{1};
    max <<= 256;

    SHE she{deg, max};
    she.genKey(0);


    cout << "Calul max degré : ";
    cout.flush();
    int degmax = 0;
    for(int i = 0; i < 1000; i++) {
        bool b;
        if (!(b = she.testPolynomial(2 + i, 0))) break;
        //else cout << boolalpha << b << endl;

        if (!(b = she.testPolynomial(2 + i, 1))) break;
        //else cout << boolalpha << b << endl;
        degmax++;
        if(i % 10 == 0) {
            cout << ". ";
            cout.flush();
        }
    }

    cout << endl;
    cout << "Degré Max : " << degmax << endl;
    cout << endl;

    int deg_rec = 0;

    int b1 = rand() % 2;
    Cipher c1 = she.encrypt(b1);
    Cipher r1 = she.recrypt(c1);
    mpz_class b2 = she.decrypt(r1);

   // cout << b1 << " == " << b2 << " " << boolalpha << (b1 == b2) << endl;
    deg_rec = r1.getNbTimes();
    cout << "Degré Recrypt = " << deg_rec << endl;

    
    int max_iter = 1000;
    int m = 0;

    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, rand());

    mpz_class d = she.get_d();

    for(int i = 2; i < max_iter; i++) {
        Polynomial p{i, 2, state};
        char b = rand() % 2;
        Cipher c = she.encrypt(b);

        mpz_class r1 = p.evalmod(b, 2) & 1;
        mpz_class r2 = p.eval(c.getValue()) % d;
        if (r2 >=  d / 2) r2 -= d;
        if (r2 < - d / 2) r2 += d; 

        Cipher cb{&she, r2};
        Cipher cr = she.recrypt(cb);
        cout << "nr = " << she.getNorm(cr.getValue()) << endl;
        cout << "nb = " << she.getNorm(cb.getValue()) << endl;
        mpz_class diff = she.getNorm(cb.getValue()) - she.getNorm(cr.getValue());
        //cout << "diff norme = " << diff << endl;

        mpz_class bd = she.decrypt(cb);
        mpz_class r = she.decrypt(cr);
        if(bd != r1) {
            cout << "END Dec" << endl;
            break;
        }
        if(r != r1) {
            cout << "END Rec" << endl;
            break;
        }

        m = i;
    }
      
    cout << "Max degree before recrypt : " << m << endl;

    gmp_randclear(state);
}

int minimum(vector<int> l) {
    int m = l[0];
    for (unsigned int i = 1; i < l.size(); i++)
        if (l[i] < m) m = l[i];
    return m;
}

void test_rdec() {
    // Paramètres du SHE
    int deg = 6;
    mpz_class max{1};
    max <<= 256;

    SHE she{deg, max};
    she.genKey(0);

    // Calcul du rDec
    mpz_class rdec = she.getRDec();

    // On ouvre le fichier de sortie
    ofstream outfile;
    outfile.open("out/plot.out", ios::trunc);

    // On écrit rDec dans le fichier
    outfile << rdec <<  "\n";
    outfile << rdec/ (NB_KEY + 1) << "\n";

    // On démarre une horloge qui définit les abscisses de notre graphique.
    clock_t start = clock();

    // On créer un chiffré qu'on ne va pas toucher pour le moment
    int b1 = rand() % 2;
    Cipher c1 = she.encrypt(b1);
    outfile << float(clock() - start) / CLOCKS_PER_SEC << "," << she.getNorm(c1.getValue()) << "\n";

    // On créer un deuxième chiffré
    int b2 = rand() % 2;
    Cipher c2 = she.encrypt(b2);

    // Pour générer le polynome
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, rand());

    // Nb d'iteration max
    int max_iter = 100;
    mpz_class d = she.get_d();

    vector<int> nb_times{};
    for(int i = 0; i < max_iter; i++) {
        Polynomial P{80,2, state};

        mpz_class r1 = P.evalmod(b2, 2) & 1; // P(b2)
        mpz_class r2 = P.eval(c2.getValue()); // P(c2)

        Cipher c3{&she, r2};
        Cipher prod = c1 + c3; // c1 * P(c2);
        Cipher prodrc = she.recrypt(prod);

        mpz_class bd = she.decrypt(prod); 
        mpz_class r = she.decrypt(prodrc); 

        // Il faut bd = b1 * P(b2) Sinon on recrypt les données de base
        if ((bd & 1) != ((b1^r1)&1)) {
            nb_times.push_back(c1.getNbTimes());
            nb_times.push_back(c3.getNbTimes());
            c1 = she.recrypt(c1);
            c3 = she.recrypt(c3);
            prod = c1 + c3;
        }

        // Il faut r = b1 * P(b2) Sinon on recrypt les données de base
        if ((r & 1) != ((b1^r1)&1)) {
            nb_times.push_back(c1.getNbTimes());
            nb_times.push_back(c3.getNbTimes());
            c1 = she.recrypt(c1);
            c3 = she.recrypt(c3);
            prod = c1 + c3;
        }

        if (she.getNorm(prod.getValue()) > (rdec)) {
            nb_times.push_back(c1.getNbTimes());
            nb_times.push_back(c3.getNbTimes());
            c1 = she.recrypt(c1);
            c3 = she.recrypt(c3);
            prod = c1 + c3;
        }

        outfile << float(clock() - start) / CLOCKS_PER_SEC << "," << she.getNorm(prod.getValue()) << "\n";
        c1 = prod;
    }

    // On ferme le fichier
    outfile.close();

    cout << "Minimum mul avant recrypt : " << minimum(nb_times) << endl;
    gmp_randclear(state);

    // On execute python pour tracer les graphiques
    execlp("python3", "python3", "src/plot.py", NULL);
}


void test_graph() {
    // Paramètres du SHE
    int deg = 6;
    mpz_class max{1};
    max <<= 256;

    SHE she{deg, max};
    she.genKey(1);

    // Calcul du rDec
    mpz_class rdec = she.getRDec();

    // On ouvre le fichier de sortie
    ofstream outfile;
    outfile.open("out/plot.out", ios::trunc);

    // On écrit rDec dans le fichier
    outfile << rdec <<  "\n";
    outfile << rdec/ (NB_KEY + 1) << "\n";

    // On créer un chiffré qu'on ne va pas toucher pour le moment
    int b = rand() % 2;
    Cipher c = she.encrypt(b);
    outfile << she.getNorm(c.getValue()) << " *" << "\n";

    // Nb d'iteration max
    int max_iter = 100;
    mpz_class d = she.get_d();

    vector<int> nb_times{};
    for(int i = 0; i < max_iter; i++) {
        Cipher cc = she.encrypt(b);
        Cipher ccc = c * cc;
        for(int j = 1; j < 5; j++) {
            ccc = ccc * cc;
        }
        if(she.getNorm(ccc.getValue()) >= rdec / (NB_KEY + 1)) {
            nb_times.push_back(c.getNbTimes());
            c = she.recrypt(c, outfile);
            c.setNbTimes(1);
            cout << "REC NORM" << endl;
        } else if((she.decrypt(ccc) & 1) != (b & 1)) {
            nb_times.push_back(c.getNbTimes());
            c = she.recrypt(c, outfile);
            c.setNbTimes(1);
            cout << "REC DEC" << endl;
        } else if ((she.decrypt(she.recrypt(ccc)) & 1) != (b & 1)) {
            nb_times.push_back(c.getNbTimes());
            c = she.recrypt(c, outfile);
            c.setNbTimes(1);
            cout << "REC REC" << endl;
        } else {
            c = ccc;
            outfile << she.getNorm(c.getValue()) << " *" << "\n";
            cout << she.getNorm(c.getValue()) << endl;
        }
    }

    int oldb = b;

    for(int i = 0; i < max_iter; i++) {
        Cipher cc = she.encrypt(b);
        Cipher ccc = c + cc;
        b ^= oldb;
        for(int j = 1; j < 5; j++) {
            ccc = ccc + cc;
            b ^= oldb;
        }
        if(she.getNorm(ccc.getValue()) >= rdec / (NB_KEY + 1)) {
            //nb_times.push_back(c.getNbTimes());
            c = she.recrypt(c, outfile);
            b = oldb;
        } else if((she.decrypt(ccc) & 1) != (b & 1)) {
            //nb_times.push_back(c.getNbTimes());
            c = she.recrypt(c, outfile);
            b = oldb;
        } else if ((she.decrypt(she.recrypt(ccc)) & 1) != (b & 1)) {
            //nb_times.push_back(c.getNbTimes());
            c = she.recrypt(c, outfile);
            b = oldb;
        } else {
            c = ccc;
            outfile << she.getNorm(c.getValue()) << " +" << "\n";
            oldb = b;
            cout << she.getNorm(c.getValue()) << endl;
        }
    }

    for(int i = 0; i < max_iter; i++) {
        Cipher ccc = c + c;
        b ^= oldb;
        for(int j = 1; j < 5; j++) {
            ccc = ccc + c;
            b ^= oldb;
        }
        if(she.getNorm(ccc.getValue()) >= rdec / (NB_KEY + 1)) {
            //nb_times.push_back(c.getNbTimes());
            c = she.recrypt(c, outfile);
            b = oldb;
        } else if((she.decrypt(ccc) & 1) != (b & 1)) {
            //nb_times.push_back(c.getNbTimes());
            c = she.recrypt(c, outfile);
            b = oldb;
        } else if ((she.decrypt(she.recrypt(ccc)) & 1) != (b & 1)) {
            //nb_times.push_back(c.getNbTimes());
            c = she.recrypt(c, outfile);
            b = oldb;
        } else {
            c = ccc;
            outfile << she.getNorm(c.getValue()) << " !" << "\n";
            oldb = b;
            cout << she.getNorm(c.getValue()) << endl;
        }
    }

    // On ferme le fichier
    outfile.close();

    cout << "Minimum mul avant recrypt : " << minimum(nb_times) << endl;

    // On execute python pour tracer les graphiques
    execlp("python3", "python3", "src/plot.py", NULL);
}

void test_time() 
{
    mpz_class max{1};
    max <<= 256;
    SHE she1{5, max};
    SHE she2{6, max};
    SHE she3{7, max};
    SHE she4{8, max};
    SHE she5{9, max};
    SHE she6{10, max};

    ofstream outfile; 
    outfile.open("time.out", ios::trunc);

    struct timespec start, finish;
    double elapsed;


    clock_gettime(CLOCK_MONOTONIC, &start);
    she1.genKey(1);
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    cout << elapsed << endl;
    outfile << elapsed << endl;

    clock_gettime(CLOCK_MONOTONIC, &start);
    she2.genKey(1);
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    cout << elapsed << endl;
    outfile << elapsed << endl;

    clock_gettime(CLOCK_MONOTONIC, &start);
    she3.genKey(1);
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    cout << elapsed << endl;
    outfile << elapsed << endl;

    clock_gettime(CLOCK_MONOTONIC, &start);
    she4.genKey(1);
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    cout << elapsed << endl;
    outfile << elapsed << endl;

    clock_gettime(CLOCK_MONOTONIC, &start);
    she5.genKey(1);
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    cout << elapsed << endl;
    outfile << elapsed << endl;

    clock_gettime(CLOCK_MONOTONIC, &start);
    she6.genKey(1);
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    cout << elapsed << endl;
    outfile << elapsed << endl;

    outfile.close();
    
}

void test_newInvert()
{
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, rand());
    
    cout << "==== TEST CRT ====" << endl;


    mpz_class big = 1;
    big <<= 50;
    cout << big << endl;
    mpz_class prime1, prime2;
    mpz_nextprime (prime1.get_mpz_t(), big.get_mpz_t());
    mpz_nextprime (prime2.get_mpz_t(), prime1.get_mpz_t());
    cout << prime1 << " " << prime2 << endl;

    mpz_class a, b;
    big << 50;
    mpz_urandomm(a.get_mpz_t(), state, big.get_mpz_t());
    mpz_urandomm(b.get_mpz_t(), state, big.get_mpz_t());

    cout << a << " " << b << endl;
    
    a %= prime1;
    b %= prime2;

    mpz_class c = CRT(a, prime1, b, prime2);
    if(c < 0) c += prime1 * prime2;
    if(a < 0) a += prime1;
    if(b < 0) b += prime2;
    cout << a << " == " << c % prime1 << " " << b << " == " << c % prime2 << endl; 


    cout << "==== TEST POLYNOME ====" << endl;
    int deg = 1 << 8;

    mpz_class max{1};
    max <<= 100;
    Polynomial p1{deg - 1, max, state};

    Polynomial p2{deg};
    p2[deg] = 1;
    p2[0] = 1;
    
    cout << "p1 : " << p1 << endl;
    cout << "p2 : " << p2 << endl;

    Polynomial w;
    mpz_class d;

    tie(w, d) = invert_Polynomial(p1, p2, 100);

    //cout << "w = " << w << endl;
    cout << "d = " << d << endl;
    cout << "w1 = " << w[1] << endl; 

    // Polynomial q, r;
    // Polynomial p3 = p1 * w;
    // mpz_class aa;
    // tie(aa, q, r) = p3.EuclidianDiv(p2);
    // cout << r << endl;


    Polynomial G, U, V;
    tie(G, U, V) = p2.Bezout(p1);
    cout << "**d = " << G << endl;
    //cout << "**w = " << V << endl;
    cout << "**w0 = " << V[0] << endl;
    cout << "**w1 = " << V[0] << endl; 

    mpz_class r1 = d / G[0];
    mpz_class r2 = w[0] / V[0];
    mpz_class r3 = w[1] / V[1];
    cout << "r1 = " << r1 << endl;
    cout << "r2 = " << r2 << endl;
    cout << "r3 = " << r3 << endl;
    mpz_class gcd1;
    mpz_class gcd;
    mpz_gcd(gcd1.get_mpz_t(), d.get_mpz_t(), w[0].get_mpz_t());
    mpz_gcd(gcd.get_mpz_t(),gcd1.get_mpz_t(), w[1].get_mpz_t());
    cout << "gcd = " << gcd << endl;

    bool b1 = ((r1 * G[0]) == d);
    bool b2 = ((r2 * V[0]) == w[0]);
    bool b3 = ((r3 * V[1]) == w[1]);
    cout << boolalpha << b1 << endl;
    cout << boolalpha << b2 << endl;
    cout << boolalpha << b3 << endl;


    gmp_randclear(state);
}

void test_1()
{
    int deg = 6;
    mpz_class max{1};
    max <<= 256;
    SHE she{deg, max};
    she.genKey(0);

     cout << she << endl;

    cout << "==== Encrypt / Decrypt un bit ====" << endl;
    cout << "-- 0 --" << endl; 
    Cipher e0 = she.encrypt(0);
    cout << "e0 = " << e0 << endl;
    mpz_class d0 = she.decrypt(e0);
    cout << "d(e0) = " << d0 << endl; 

    cout << "-- 1 --" << endl;
    Cipher e1 = she.encrypt(1);
    cout << "e1 = " << e1 << endl;
    mpz_class d1 = she.decrypt(e1);
    cout << "d(e1) = " << d1 << endl; 

    cout << "==== Somme de deux chiffrés ====" << endl;
    cout << "-- 0 XOR 0 --" << endl;
    Cipher c1 = she.encrypt(0);
    Cipher c0 = she.encrypt(0);
    Cipher s = c1 + c0;
    mpz_class d = she.decrypt(s);
    cout << "d(c1 + c0) = " << d <<  endl;

    cout << "-- 0 XOR 1 --" << endl;
    c1 = she.encrypt(0);
    c0 = she.encrypt(1);
    s = c1 + c0;
    d = she.decrypt(s);
    cout << "d(c1 + c0) = " << d <<  endl;

    cout << "-- 1 XOR 0 --" << endl;
    c1 = she.encrypt(1);
    c0 = she.encrypt(0);
    s = c1 + c0;
    d = she.decrypt(s);
    cout << "d(c1 + c0) = " << d <<  endl;

    cout << "-- 1 XOR 1 --" << endl;
    c1 = she.encrypt(1);
    c0 = she.encrypt(1);
    s = c1 + c0;
    d = she.decrypt(s);
    cout << "d(c1 + c0) = " << d <<  endl;

    cout << "==== Produit de deux chiffrés ====" << endl;
    cout << "-- 0 AND 0 --" << endl;
    c1 = she.encrypt(0);
    c0 = she.encrypt(0);
    s = c1 * c0;
    d = she.decrypt(s);
    cout << "d(c1 * c0) = " << d << endl;

    cout << "-- 0 AND 1 --" << endl;
    c1 = she.encrypt(0);
    c0 = she.encrypt(1);
    s = c1 * c0;
    d = she.decrypt(s);
    cout << "d(c1 * c0) = " << d << endl;


    cout << "-- 1 AND 0 --" << endl;
    c1 = she.encrypt(1);
    c0 = she.encrypt(0);
    s = c1 * c0;
    d = she.decrypt(s);
    cout << "d(c1 * c0) = " << d << endl;

    cout << "-- 1 AND 1 --" << endl;
    c1 = she.encrypt(1);
    c0 = she.encrypt(1);
    s = c1 * c0;
    d = she.decrypt(s);
    cout << "d(c1 * c0) = " << d << endl;
}


void test_2()
{
    int deg = 6;
    mpz_class max{1};
    max <<= 256;
    SHE she{deg, max};
    she.genKey(0);

    Cipher c0 = she.encrypt(0);
    Cipher c1 = she.encrypt(0);

    int i = 0;

    while(true)
    {
        i++;
        c0 = c0 * c1;
        mpz_class dec = she.decrypt(c0);
        cout << "0 AND 0 = " << dec << endl;

        if((dec & 1) != 0) break;
    }

    cout << "Total AND = " << i << endl;

}

int main(int argc, char *argv[]) 
{
    srand(time(NULL));
    for(int i = 1; i < argc; i ++) {
        std::string command(argv[i]);

        if (command == "mpz")
            test_mpz();

        else if (command == "polynomials")
            test_polynomials();

        else if (command == "she")
            test_SHE();

        else if (command == "shem")
            test_SHEM();

        else if (command == "gsa")
            test_GSA();

        else if (command == "squash")
            test_squash();

        else if (command == "recrypt")
            test_recrypt();

        else if (command == "sum")
            test_sum_int();

        else if (command == "thread")
            test_thread();

        else if (command == "invert")
            test_invert_pol();
        
        else if (command == "mod")
            test_mod();

        else if (command == "di")
            test_di();

        else if (command == "deg_rec")
            test_deg_recrypt();

        else if (command == "rdec")
            test_rdec();

        else if (command == "new_invert")
            test_newInvert();
        else if (command == "graph")
            test_graph();
        else if(command == "time")
            test_time();
        else if(command == "test_1")
            test_1();
        else if(command == "test_2")
            test_2();
    }

    return 0;
}
