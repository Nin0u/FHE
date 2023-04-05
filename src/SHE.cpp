#include "SHE.hpp"
#include <tuple>

using namespace std;

/**
 * Constructeur
 * Construit le polynome X^(2^n) + 1
 */
SHE::SHE(int n) : deg{1 << n}, polMod{1 << n}
{
    polMod[0] = BigInt{1};
    polMod[(1<< n)] = BigInt{1};  
}

/** Générateur de clé */
int SHE::genKeyCandidate()
{
    v = Polynomial{deg - 1, BigInt{100}};
    Polynomial G,U,V;
    tie(G,U,V) = polMod.Bezout(v);
    if (G.getDegree() != 0) return 0;
    BigInt d  = G[0];
    if (d < 0) {
        d = d * BigInt{-1};
        V = V * BigInt{-1};
    }
    if (d % BigInt{2} == BigInt{0}) return 0;
    int index_odd_coeff = V.hasOddCoeff();
    if (index_odd_coeff == -1) return 0;
    BigInt g,v1,v2;
    cout << "d1 = ";
    d.write(cout) << endl;
    cout << "v1 = ";
    V[1].write(cout) << endl;


    tie(g, v1, v2) = BigInt::xgcd(d,V[1]);
    cout << "d2 = ";
    d.write(cout) << endl;
    cout << "v2 = ";
    V[1].write(cout) << endl;
    if(g != BigInt{1}) return 0;
    r = (V[0] * v1) % d;
    if(r < 0) r *= BigInt{-1};
    cout << "D = ";
    d.write(cout) << endl;
    cout << "R = ";
    r.write(cout) << endl;
    BigInt res = (r.powmod(BigInt{deg}, d) + BigInt{1}) % d;
    //res.write(cout) << endl;
    if(res != BigInt{0}) return 0; 
    this->d = d;
    this->r = r;
    w = V;
    wi = V[index_odd_coeff];
    return 1;
}

void SHE::genKey() { while(!genKeyCandidate()); }

/** Méthode d'affichage */
ostream &operator<<(ostream &out, const SHE& she){
    out << "V = " << she.v << endl;
    out << "W = " << she.w << endl;
    out << "d = ";
    she.d.write(out) << endl;
    out << "r = ";
    she.r.write(out) << endl;
    out << "wi = ";
    she.wi.write(out) << endl;

    return out;
}