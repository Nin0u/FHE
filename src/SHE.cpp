#include "SHE.hpp"
#include <iostream>
#include <tuple>
#include <unistd.h>

using namespace std;

/**
 * Constructeur
 * Construit le polynome X^(2^n) + 1
 */
SHE::SHE(int n) : deg{1 << n}, polMod{1 << n}, state{}
{
    polMod[0] = mpz_class{1};
    polMod[deg] =  mpz_class{1}; 
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, rand());
}

/** 
 * Destructeur 
 * On doit libérer le générateur de nombre
 */
SHE::~SHE(){ 
    gmp_randclear(state);
}

/** Générateur de clé */
int SHE::genKeyCandidate()
{   
    v = Polynomial{deg - 1, 100, state};
    Polynomial G,U,V;
    tie(G,U,V) = polMod.Bezout(v);

    if (G.getDeg() != 0) return 0;

    mpz_class d = G[0];
    if (d < 0) {
        mpz_neg(d.get_mpz_t(), d.get_mpz_t());
        V = V *  mpz_class{-1};
    }
    
    if (d % 2 == 0) return 0;

    int index_odd_coeff = V.hasOddCoeff();
    if (index_odd_coeff == -1) return 0;

    mpz_class g,v1,v2;
    mpz_gcdext(g.get_mpz_t(), v1.get_mpz_t() ,v2.get_mpz_t(), V[1].get_mpz_t(), d.get_mpz_t());
    if(g != 1) return 0;
    r = (V[0] * v1) % d;

    mpz_class res;
    mpz_powm(res.get_mpz_t() , r.get_mpz_t() , mpz_class{deg}.get_mpz_t() , d.get_mpz_t());
    mpz_add(res.get_mpz_t(), res.get_mpz_t(), mpz_class{1}.get_mpz_t());
    mpz_mod(res.get_mpz_t(), res.get_mpz_t(), d.get_mpz_t());

    if(res != 0) return 0; 
    
    this->d = d;
    this->r = r;
    w = V;
    wi = V[index_odd_coeff];
    return 1;
}

void SHE::genKey() { while(!genKeyCandidate()) sleep(2); }

/** Encryption */
mpz_class SHE::encrypt(char bit){
    Polynomial b{deg - 1};
    b[0] = bit & 1;

    // u est un vecteur à valeurs dans [-1 , 1]
    Polynomial u{deg - 1, 2, state};
    u = u * mpz_class{2};

    Polynomial a = b + u;
    mpz_class c = a.eval(r,d);
    if (c > (d / 2)) c -= d;

    return c;
}

/** Decryption */
mpz_class SHE::decrypt(mpz_class text){
    mpz_class decrypt_coeff = wi;
    mpz_class m = (decrypt_coeff * text) % d;
    if (m >= (d / 2)) m -= d;
    if (m <= -(d / 2)) m += d;
    mpz_class b = m % 2;
    return b & 1;
}

/** Méthode d'affichage */
ostream &operator<<(ostream &out, const SHE& she){
    out << "V = " << she.v << endl;
    out << "W = " << she.w << endl;
    out << "d = " << she.d << endl;
    out << "r = " << she.r << endl;
    out << "wi = " << she.wi << endl;

    return out;
}