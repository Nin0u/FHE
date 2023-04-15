#include "SHE.hpp"
#include "Algo.hpp"
#include <iostream>
#include <tuple>
#include <unistd.h>



using namespace std;

/**
 * Constructeur
 * Construit le polynome X^(2^n) + 1
 */
SHE::SHE(int n, mpz_class max_v) : deg{1 << n}, polMod{1 << n}, state{}, max_v{max_v}
{
    polMod[0] = mpz_class{1};
    polMod[deg] =  mpz_class{1}; 

    for(int i = 0; i < NB_KEY; i++) {
        for(int j = 0; j < NB_ELEM; j++) {
            X [i][j] = 0;
            sk[i][j] = 0;
            ck[i][j] = 0;
        }
    }

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

void SHE::splitKey()
{
    mpz_class split_sk[NB_KEY];
    mpz_class sum = 0;
    for(int i = 0; i < NB_KEY - 1; i++) {
        mpz_urandomm(split_sk[i].get_mpz_t(), state, d.get_mpz_t());
        if(split_sk[i] >= d / 2) split_sk[i] -= d / 2;
        sum += split_sk[i];
    }

    split_sk[NB_KEY - 1] = (wi - sum) % d;
    if(split_sk[NB_KEY - 1] >= d / 2) split_sk[NB_KEY - 1] -= d;

    for(int i = 0; i < NB_KEY; i++) {
        int index = rand() % NB_ELEM;
        sk[i][index] = 1;
        for(int j = 0; j < NB_ELEM; j++) {
            if(j != index) {
                mpz_urandomm(X[i][j].get_mpz_t(), state, d.get_mpz_t());
                if(X[i][j] >= d / 2) {
                    X[i][j] -= d;
                }
            } else {
                X[i][j] = split_sk[i];
            }
        }
    }

    // for(int i = 0; i < NB_KEY; i++) {
    //     for(int j = 0; j < NB_ELEM; j++) {
    //         cout << sk[i][j] << " ";
    //     }
    //     cout << endl;
    // }


    for(int i = 0; i < NB_KEY; i++) {
        for(int j = 0; j < NB_ELEM; j++) {
            ck[i][j] = encrypt(sk[i][j] & 1);
        }
    }
}

void SHE::genKey() 
{ 
    while(!genKeyCandidate()); 
    splitKey();
}

/** Encryption */
mpz_class SHE::encrypt(mpz_class bit){
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
            if(!minus) rep[i] += v[k] * vec[j];
            else rep[i] -= v[k] * vec[j];
            k--;
            if(k < 0) {
                minus = true;
                k += deg;
            }
        }
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
    //cout << "P = " << p << endl;
    mpz_class c = encrypt(b);

    mpz_class r1 = p.evalmod(b, 2) & 1;
    mpz_class r2 = p.eval(c) % d;
    if (r2 >=  d / 2) r2 -= d;
    if (r2 < - d / 2) r2 += d; 


    mpz_class d1 = decrypt(r2);

    //cout << "r1 = " << r1 << endl;
    //cout << "d1 = " << d1 << endl;

    return r1 == d1;
}

// Fonction renvoyant le ciphertext étendu neccessaire pour le Decrypt Squash
vector<vector<mpz_class>> SHE::expandCT(mpz_class text) 
{
    vector<vector<mpz_class>> res{};
    res.resize(NB_KEY);
    for(int i = 0; i < NB_KEY; i++) {
        res[i].resize(NB_ELEM);
        for(int j = 0; j < NB_ELEM; j++) {
            res[i][j] = ((text * X[i][j]) % d);
            if(res[i][j] < 0) res[i][j] += d;
        }
    }
    return res;
}

// Premiere Version du Decrypt Squash, pour tester si le expandCT et le splitKey fonctionne
mpz_class SHE::decrytpSquash(vector<vector<mpz_class>> text)
{
    mpz_class res = 0;
    for(int i = 0; i < NB_KEY; i++) {
        for(int j = 0; j < NB_ELEM; j++) {
            res = (res + (sk[i][j] * text[i][j])) % d;
        }
    }
    if(res < -d / 2) res += d;
    if(res >= d / 2) res -= d;
    return res & 1;
}

// Le vrai decrypt Squash
mpz_class SHE::decrytpRealSquash(vector<vector<mpz_class>> text)
{

    // La partie Gauche le somme facile à calculer, juste une double Xor
    mpz_class left = 0;
    for(int i = 0; i < NB_KEY; i++) {
        for(int j = 0; j < NB_ELEM; j++) {
            left ^= (text[i][j] & 1) & (sk[i][j]);
        }
    }

    vector<mpz_class> vec{};
    vec.resize(NB_KEY);

    //TODO: log2(NB_KEY) + 1 !
    //Nombre de bit apres la virgule
    int precision = 7;

    // On calcule les Xor de la partie droite
    // On stocke ça dans v
    for(int i = 0; i < NB_KEY; i++) {
        vec[i] = 0;
        for(int j = 0; j < NB_ELEM; j++) {
            vec[i] ^= (sk[i][j] * ((text[i][j] << precision) / d));
        }
    }

    vector<vector<mpz_class>> columns{};
    columns.resize(precision + 1);
    for(int i = 0; i < precision + 1; i++) {
        columns[i].resize(NB_KEY);
    }

    // On prépare les colonnes pour le GSA
    for(int j = 0; j < NB_KEY; j++) {
        for(int i = 0; i < precision + 1; i++) {
            columns[i][j] = (vec[j] >> i) & 1;
        }
    }

    // On applique le GSA
    vec = gradeSchoolAddition(columns);

    // Pour faire le Round + Mod 2, il suffit de faire le Xor entre le bit avant la virgule et celui apres
    mpz_class right = vec[precision] ^ vec[precision - 1];

    // On renvoie le Xor final
    return left ^ right;
}


mpz_class SHE::decrytpRealSquash2(vector<vector<mpz_class>> text)
{

    // La partie Gauche le somme facile à calculer, juste une double Xor
    mpz_class left = 0;
    for(int i = 0; i < NB_KEY; i++) {
        for(int j = 0; j < NB_ELEM; j++) {
            left ^= (text[i][j] & 1) & (sk[i][j]);
        }
    }

    //TODO: log2(NB_KEY) + 1 !
    //Nombre de bit apres la virgule
    int precision = 7;


    mpz_class matrix[NB_KEY][NB_ELEM][precision + 1];

    for(int i = 0; i < NB_KEY; i++) {
        for(int j = 0; j < NB_ELEM; j++) {
            mpz_class t = (text[i][j] << precision) / d;
            for(int k = 0; k < precision + 1; k++) {
                //matrix[i][j][k] = encrypt((t >> k) & 1);
                matrix[i][j][k] = ((t >> k) & 1) & (sk[i][j]);
            } 
        }
    }

    //Premier Xor de Droite
    mpz_class vec [NB_KEY][precision + 1];
    for(int i = 0; i < NB_KEY; i++) {
        for(int k = 0; k < precision + 1; k++) {
            vec[i][k] = 0;
        }
        for(int j = 0; j < NB_ELEM; j++) {
            for(int k = 0; k < precision + 1; k++) {
                vec[i][k] += matrix[i][j][k];
            }
        }
    }   
    
    // On Créer les colonnes pour le grade school
    vector<vector<mpz_class>> columns{};
    columns.resize(precision + 1);
    for(int i = 0; i < precision + 1; i++) {
        columns[i].resize(NB_KEY);
    }

    for(int j = 0; j < NB_KEY; j++) {
        for(int i = 0; i < precision + 1; i++) {
            columns[i][j] = vec[j][i];
        }
    }    

    vector<mpz_class> rep = gradeSchoolAddition(columns);
    mpz_class right = rep[precision] ^ rep[precision - 1];

    return left ^ right;

}



mpz_class SHE::recrypt(mpz_class ctext)
{
    vector<vector<mpz_class>> text = expandCT(ctext);

    // La partie Gauche le somme facile à calculer, juste une double Xor
    mpz_class left = 0;
    for(int i = 0; i < NB_KEY; i++) {
        for(int j = 0; j < NB_ELEM; j++) {
            left = addCipher(left, mulCipher(text[i][j] & 1, ck[i][j]));
        }
    }

    //TODO: log2(NB_KEY) + 1 !
    //Nombre de bit apres la virgule
    int precision = 5;

    // On calcule les Xor de la partie droite
    mpz_class matrix[NB_KEY][NB_ELEM][precision + 1];

    for(int i = 0; i < NB_KEY; i++) {
        for(int j = 0; j < NB_ELEM; j++) {
            mpz_class t = (text[i][j] << precision) / d;
            for(int k = 0; k < precision + 1; k++) {
                matrix[i][j][k] = mulCipher((t >> k) & 1, ck[i][j]);
            } 
        }
    }

    //Premier Xor de Droite
    mpz_class vec [NB_KEY][precision + 1];
    for(int i = 0; i < NB_KEY; i++) {
        for(int k = 0; k < precision + 1; k++) {
            vec[i][k] = 0;
        }
        for(int j = 0; j < NB_ELEM; j++) {
            for(int k = 0; k < precision + 1; k++) {
                vec[i][k] = addCipher(vec[i][k], matrix[i][j][k]);
            }
        }
    }   
    
    // On Créer les colonnes pour le grade school
    vector<vector<mpz_class>> columns{};
    columns.resize(precision + 1);
    for(int i = 0; i < precision + 1; i++) {
        columns[i].resize(NB_KEY);
    }

    for(int j = 0; j < NB_KEY; j++) {
        for(int i = 0; i < precision + 1; i++) {
            columns[i][j] = vec[j][i];
        }
    }    

    vector<mpz_class> rep = gradeSchoolAddition(columns, d);
    mpz_class right = addCipher(rep[precision], rep[precision - 1]);

    return addCipher(left, right);

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