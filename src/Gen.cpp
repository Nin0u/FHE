#include "Gen.hpp"
#include <iostream>
#include <thread>

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

mpz_class get_di(Polynomial v, mpz_class prime, int deg)
{
    int ieme = deg << 1;
    mpz_class root = get_racine_prim(prime, ieme);
    mpz_class di = 1;
    mpz_class current_root = root;

    int j = 0;
    for(int i = 0; i < ieme; i += 2) {
        //cout << "current_root = " << current_root << endl;
        mpz_class vj = v.evalmod(current_root, prime); 
        di *= vj;
        di %= prime;

        current_root *= root;
        current_root *= root;
        current_root %= prime;
        j++;
    }

    return di;
}

void CRT_di(Polynomial v, Polynomial X, mpz_class *primes, int i, int deep, mpz_class *r, mpz_class *w0, mpz_class *mod)
{
    if(deep == 0)
    {
        //cout << i << " prime : " << primes[i] << endl;

        mpz_class prime = primes[i];
        v.reduce(prime);

        mpz_class d = get_di(v, prime,X.getDeg());

        //cout << "di = " << d << endl;

        Polynomial G,U,V;
        tie(G,U,V) = X.Bezout(v, prime);
        mpz_class g = G[0];
        mpz_class g1;
        mpz_invert(g1.get_mpz_t(), g.get_mpz_t(), prime.get_mpz_t());
        mpz_class w00 = (V[0] * g1 * d) % prime;

        *r = d;
        if(w00 <= -prime / 2) w00 += prime;
        else if(w00 >= prime / 2) w00 -= prime; 
        *w0 = w00;

        *mod = prime;
        return;
    }    

    mpz_class d1, d2;
    mpz_class mod1, mod2;
    mpz_class w00, w01;
    thread t1{CRT_di, v, X, primes, i, deep - 1, &d1, &w00, &mod1};
    thread t2{CRT_di, v, X, primes, i + (1 << (deep - 1)), deep - 1, &d2, &w01, &mod2};

    t1.join();
    t2.join();

    mpz_class d = CRT(d1, mod1, d2, mod2);
    //cout << "d = " << d << endl;
    mpz_class w000 = CRT(w00, mod1, w01, mod2);
    if(w000 <= -(mod1 * mod2) / 2) w000 += mod1 * mod2;
    else if(w000 >= (mod1 * mod2) / 2) w000 -= mod1 * mod2;

    *r = d;
    *w0 = w000;
    *mod = mod1 * mod2;
    return;
}


tuple<Polynomial, mpz_class> invert_Polynomial(Polynomial v, Polynomial X, int max_thread, int max_prime)
{
    int deep = max_thread;

    mpz_class primes[1 << deep];
    mpz_class a = {1};
    a <<= max_prime;

    int deg = 2 * X.getDeg();

    mpz_class prime;
    for(int i = 0; i < 1 << deep; i++) {
        do {
            mpz_nextprime (prime.get_mpz_t(), a.get_mpz_t());
            a = prime;
        } while(a % (deg) != 1);
        //cout << "prime = 1 mod 2^" << deg << " : " << a << endl;
        primes[i] = a;
    }

    Polynomial w{X.getDeg() - 1};
    mpz_class d;
    mpz_class mod;
    mpz_class w0;


    CRT_di(v, X, primes, 0, deep, &d, &w0,&mod);
    cout << "w0 = " << w0 << endl;
    w[0] = w0;

    return tie(w, d);
}

