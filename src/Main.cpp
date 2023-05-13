#include <iostream>
#include <tuple>

#include "Polynomial.hpp"
#include "SHE.hpp"
#include "Algo.hpp"
#include "Cipher.hpp"
#include "Gen.hpp"

#include <gmpxx.h>
#include <unistd.h>
#include <thread>

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
    she.genKey();
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
    she.genKey();

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
    SHE she{6, max};
    she.genKey();

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
    she.genKey();

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
    int deg = 1 << 8;

    mpz_class max{1};
    max <<= 10;
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
    tie(w, d) = invert_Polynomial(p1, p2, 8, 50);

    //cout << "w = " << w << endl;
    cout << "d = " << d << endl;

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

    mpz_class r1 = d / G[0];
    mpz_class r2 = w[0] / V[0];
    cout << "r1 = " << r1 << endl;
    cout << "r2 = " << r2 << endl;

    bool b1 = ((r1 * G[0]) == d);
    bool b2 = ((r1 * V[0]) == w[0]);
    cout << boolalpha << b1 << endl;
    cout << boolalpha << b2 << endl;


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

    mpz_class d = get_di(p1, prime, p2.getDeg());

    cout << "di : " << d << endl;

    Polynomial G, U, V;
    tie(G, U, V) = p2.Bezout(p1);
    cout << "**d = " << G << endl;
    cout << "**di = " << G[0] % prime << endl;  

    gmp_randclear(state);
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
    }

    return 0;
}
