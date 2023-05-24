#include "Gen.hpp"
#include <iostream>
#include <thread>
#include <fstream>

using namespace std;

mpz_class CRT(mpz_class a1, mpz_class p1, mpz_class a2, mpz_class p2)
{
    mpz_class g, u1, u2;
    mpz_gcdext(g.get_mpz_t(), u1.get_mpz_t() ,u2.get_mpz_t(), p1.get_mpz_t(), p2.get_mpz_t());
    
    mpz_class new_mod = p1 * p2;

    mpz_class res = (a2 * u1 * p1 + a1 * u2 * p2) % new_mod;
    if(res < 0) res += new_mod;
    return res;
}

void CRT_polynomial(Polynomial v, Polynomial X, mpz_class *primes, int i, int deep, Polynomial *rep, mpz_class *r, mpz_class *mod)
{
    if(deep == 0)
    {
        mpz_class prime = primes[i];
        v.reduce(prime);
        Polynomial G,U,V;
        tie(G,U,V) = X.Bezout(v, prime);
        // V est le polynome recherché mais il est mod prime
        Polynomial a = v * V + U * X;
        a.reduce(prime);

        mpz_class d = G[0];
        *rep = V;
        *r = d;
        *mod = prime;
        return;
    }    


    Polynomial w1, w2;
    mpz_class d1, d2;
    mpz_class mod1, mod2;
    thread t1{CRT_polynomial, v, X, primes, i, deep - 1, &w1, &d1, &mod1};
    thread t2{CRT_polynomial, v, X, primes, i + (1 << (deep - 1)), deep - 1, &w2, &d2, &mod2};

    t1.join();
    t2.join();

    mpz_class d = CRT(d1, mod1, d2, mod2);

    Polynomial w{X.getDeg() - 1};
    for(int j = 0; j <= X.getDeg() - 1; j++) {
        w[j] = CRT(w1[j], mod1, w2[j], mod2);
    }

    *rep = w;
    *r = d;
    *mod = mod1 * mod2;
    return;
}

bool test_racine_prim(mpz_class prime, int ieme, mpz_class root)
{
    mpz_class ieme2 = ieme >> 1;
    mpz_class test;
    mpz_powm (test.get_mpz_t(), root.get_mpz_t(),ieme2.get_mpz_t(), prime.get_mpz_t());
    //cout << "Test : " << test << " , prime - 1 : " << prime - 1 << endl; 
    return (prime - 1 == test);
}

mpz_class get_racine_prim(mpz_class prime, int ieme)
{
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, rand());

    mpz_class gen;
    mpz_class root;
    mpz_class puiss = (prime - 1) / ieme;

    while(1)
    {
        mpz_urandomm(gen.get_mpz_t(), state, prime.get_mpz_t());
        // gen ^ ((prime - 1) / ieme) pour avoir une racine ieme de l'unite
        mpz_powm (root.get_mpz_t(), gen.get_mpz_t(), puiss.get_mpz_t(), prime.get_mpz_t());

        if(test_racine_prim(prime, ieme, root)) break;

    }

    gmp_randclear(state);

    //cout << "root = " << root << endl;
    return root;
}

tuple<mpz_class, mpz_class, mpz_class> get_di(Polynomial v, mpz_class prime, int deg)
{
    int ieme = deg << 1;
    mpz_class root = get_racine_prim(prime, ieme);
    mpz_class di = 1;
    mpz_class current_root = root;

    Polynomial v2{deg - 1};
    for(int i = 1; i < deg; i++) {
        v2[i] = v[i - 1];
    }
    v2[0] = -v[deg - 1];

    vector<mpz_class> sum{};
    sum.resize(deg);
    for(int i = 0; i < deg; i++)
        sum[i] = 1;

    vector<mpz_class> sum2{};
    sum2.resize(deg);
    for(int i = 0; i < deg; i++)
        sum2[i] = 1;

    int j = 0;
    for(int i = 0; i < ieme; i += 2) {
        //cout << "current_root = " << current_root << endl;
        mpz_class vj = v.evalmod(current_root, prime); 
        di *= vj;
        di %= prime;

        mpz_class v2j = v2.evalmod(current_root, prime);

        for(int k = 0; k < deg; k++) {
            if(k != j) {
                sum[k] *= vj;
                sum[k] %= prime;

                sum2[k] *= v2j;
                sum2[k] %= prime;
            }
        }

        current_root *= root;
        current_root *= root;
        current_root %= prime;
        j++;
    }

    mpz_class w0 = 0;
    mpz_class w1 = 0;
    for(int i = 0; i < deg; i++) 
        w0 += sum[i];

    for(int i = 0; i < deg; i++)
        w1 += sum2[i];
    
    w0 %= prime;
    w1 %= prime;

    mpz_class n = deg;
    mpz_class n1;
    mpz_invert(n1.get_mpz_t(), n.get_mpz_t(), prime.get_mpz_t());
    mpz_class test = (n * n1) % prime;
    w0 *= n1;
    w0 %= prime;
    if(w0 <= -prime / 2) w0 += prime;
    else if(w0 >= prime / 2) w0 -= prime;

    w1 *= n1;
    w1 %= prime;
    if(w1 <= -prime / 2) w1 += prime;
    else if(w1 >= prime / 2) w1 -= prime;

    return tie(di, w0, w1);
}

void CRT_di(Polynomial v, Polynomial X, mpz_class *primes, int i, int deep, mpz_class *r, mpz_class *w0, mpz_class *w1, mpz_class *mod)
{
    if(deep == 0)
    {
        mpz_class prime = primes[i];
        v.reduce(prime);

        mpz_class d, w00, w11;
        tie(d, w00, w11) = get_di(v, prime,X.getDeg());
 
        *r = d;
        *w0 = w00;
        *w1 = w11;
        *mod = prime;
        return;
    }    

    mpz_class d1, d2;
    mpz_class mod1, mod2;
    mpz_class w00, w01;
    mpz_class w10, w11;
    thread t1{CRT_di, v, X, primes, i, deep - 1, &d1, &w00, &w10, &mod1};
    thread t2{CRT_di, v, X, primes, i + (1 << (deep - 1)), deep - 1, &d2, &w01, &w11, &mod2};

    t1.join();
    t2.join();

    mpz_class d = CRT(d1, mod1, d2, mod2);
    //cout << "d = " << d << endl;
    mpz_class w000 = CRT(w00, mod1, w01, mod2);
    if(w000 <= -(mod1 * mod2) / 2) w000 += mod1 * mod2;
    else if(w000 >= (mod1 * mod2) / 2) w000 -= mod1 * mod2;

    mpz_class w111 = CRT(w10, mod1, w11, mod2);
    if(w111 <= -(mod1 * mod2) / 2) w111 += mod1 * mod2;
    else if(w111 >= (mod1 * mod2) / 2) w111 -= mod1 * mod2;

    *r = d;
    *w0 = w000;
    *w1 = w111;
    *mod = mod1 * mod2;
    return;
}


tuple<Polynomial, mpz_class> invert_Polynomial(Polynomial v, Polynomial X, int max_thread, int max_prime)
{
    int deep = max_thread;

    mpz_class primes[1 << 15];
    mpz_class a = {1};
    a <<= max_prime;

    ifstream infile{"prime.out"};

    for(int i = 0; i < 1 << 15; i++) {
        string s;
        infile >> s;
        primes[i] = mpz_class{s};
       // cout << primes[i] << endl;
    }

    Polynomial w{X.getDeg() - 1};
    mpz_class d;
    mpz_class mod;
    mpz_class w0;
    mpz_class w1;


    CRT_di(v, X, primes, 0, deep, &d, &w0, &w1,&mod);
    //cout << "w0 = " << w0 << endl;
    w[0] = w0;
    w[1] = w1;

    return tie(w, d);
}


tuple<Polynomial, mpz_class> invert_Polynomial(Polynomial v, Polynomial X, int max_prime)
{
    int deep = 10; //! Changer !

    mpz_class primes[1 << 15];
    mpz_class a = {1};
    a <<= max_prime;

    ifstream infile{"prime.out"};

    for(int i = 0; i < 1 << 15; i++) {
        string s;
        infile >> s;
        primes[i] = mpz_class{s};
       // cout << i << " " << primes[i] << endl;
    }


    Polynomial w{X.getDeg() - 1};
    mpz_class d;
    mpz_class mod;
    mpz_class w0;
    mpz_class w1;

    deep = 5;

    CRT_di(v, X, primes, 0, deep, &d, &w0, &w1,&mod);

    while(1)
    {
        cout << "deep : " << deep << endl;
        mpz_class dd;
        mpz_class modd;
        mpz_class w00;
        mpz_class w11;
        CRT_di(v, X, primes, 1 << deep, deep, &dd, &w00, &w11, &modd);
        //cout << d << " " << dd << endl;
        if(d == dd) break;
        
        mpz_class ddd = CRT(dd, modd, d, mod);
        //cout << "d = " << d << endl;
        mpz_class w000 = CRT(w00, modd, w0, mod);
        if(w000 <= -(mod * modd) / 2) w000 += mod * modd;
        else if(w000 >= (mod * modd) / 2) w000 -= mod * modd;

        mpz_class w111 = CRT(w11, modd, w1, mod);
        if(w111 <= -(mod * modd) / 2) w111 += mod * modd;
        else if(w111 >= (mod * modd) / 2) w111 -= mod * modd;

        d = ddd;
        w0 = w000;
        w1 = w111;
        mod = mod * modd;
        deep++;
    }
    //cout << "w0 = " << w0 << endl;
    w[0] = w0;
    w[1] = w1;

    return tie(w, d);
}
