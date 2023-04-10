#include "SHE.hpp"
#include <iostream>
#include <tuple>
#include <unistd.h>

#define MAX_ERROR   5

using namespace std;

/**
 * Constructeur
 * Construit le polynome X^(2^n) + 1
 */
SHE::SHE(int n, mpz_class max_v) : deg{1 << n}, polMod{1 << n}, state{}, max_v{max_v}
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
SHE::~SHE() { gmp_randclear(state); }

/** Générateur de clé */
int SHE::genKeyCandidate()
{   
    v = Polynomial{deg - 1, max_v, state};
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
// TODO : limiter le nombre de 1 / -1
mpz_class SHE::encrypt(char bit){
    Polynomial b{deg - 1};
    b[0] = bit & 1;

    // u est un vecteur à valeurs dans [-1 , 1]
    Polynomial u{deg - 1};
    int count = 0;
    do
    {
        int index = rand() % deg;
        if(u[index] == 0) {
            int num = rand() % 2;
            if(num == 0) num = -1;
            u[index] = num;
            count++;
        }
    } while(count < MAX_ERROR);
    u = u * mpz_class{2};
    Polynomial a = b + u;
    mpz_class c = a.evalmod(r,d);
    if (c >=   d / 2) c -= d;
    if (c < - d / 2) c += d;

    return c;
}

mpz_class SHE::encryptM(vector<char> bits)
{
    Polynomial b{deg - 1};
    for(unsigned int i = 0; i < bits.size(); i++)
        b[i] = bits[i] & 1;
    
    Polynomial u{deg - 1};
    int count = 0;
    do
    {
        int index = rand() % (deg);
        if(u[index] == 0) {
            int num = rand() % 2;
            if(num == 0) num = -1;
            u[index] = num;
            count++;
        }
    } while(count < MAX_ERROR);
    u = u * mpz_class{2};
    Polynomial a = b + u;
    mpz_class c = a.evalmod(r,d);
    if (c >=   d / 2) c -= d;
    if (c < - d / 2) c += d;

    return c;
}

/** Decryption */
mpz_class SHE::decrypt(mpz_class text) {
    mpz_class decrypt_coeff = wi;
    mpz_class m = (decrypt_coeff * text) % d;
    if (m >=  d / 2) m -= d;
    if (m < - d / 2) m += d;
    mpz_class b = m % 2;
    return b & 1;
}

vector<mpz_class> SHE::decryptM(mpz_class text)
{
    vector<mpz_class> vec{};
    vec.resize(deg);

    for(int i = 0; i < deg; i++) {
        vec[i] = (w[i] * text) % d;
        if(vec[i] < - d / 2) vec[i] += d;
        if(vec[i] >=  d / 2) vec[i] -= d;
    }

    vector<mpz_class> rep{};
    rep.resize(deg);

    for(int i = 0; i < deg; i++) {
        rep[i] = 0; 
        bool minus = false;
        int k = i;
        for(int j = 0; j < deg; j++) {
            //cout << "v" << k << " * c" << j << " ";
            if(!minus) rep[i] += v[k] * vec[j];
            else rep[i] -= v[k] * vec[j];
            k--;
            if(k < 0) {
                minus = true;
                k += deg;
            }
        }
        //cout << endl;
    }

    vector<mpz_class> final{};
    final.resize(deg);
    for(int i = 0; i < deg; i++) {
        final[i] = (rep[i] & 1);
    }
    return final;
}

mpz_class SHE::addCipher(mpz_class c1, mpz_class c2) {
    mpz_class res = (c1 + c2) % d;
    if (res >=  d / 2) res -= d;
    if (res < - d / 2) res += d; 

    return res;
}
mpz_class SHE::mulCipher(mpz_class c1, mpz_class c2) {
    mpz_class res = (c1 * c2) % d;
    if (res >=  d / 2) res -= d;
    if (res < - d / 2) res += d; 

    return res;
}

bool SHE::testPolynomial(int deg, char b) {
    Polynomial p{deg, 2, state};
    cout << "P = " << p << endl;
    mpz_class c = encrypt(b);

    mpz_class r1 = p.evalmod(b, 2) & 1;
    mpz_class r2 = p.eval(c) % d;
    if (r2 >=  d / 2) r2 -= d;
    if (r2 < - d / 2) r2 += d; 


    mpz_class d1 = decrypt(r2);

    cout << "r1 = " << r1 << endl;
    cout << "d1 = " << d1 << endl;

    return r1 == d1;
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