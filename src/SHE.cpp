#include "SHE.hpp"
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
            X[i][j] = 0;
            sk[i][j] = 0;
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

    for(int i = 0; i < NB_KEY; i++) {
        for(int j = 0; j < NB_ELEM; j++) {
            cout << sk[i][j] << " ";
        }
        cout << endl;
    }
}

void SHE::genKey() 
{ 
    while(!genKeyCandidate()); 
    splitKey();
}

/** Encryption */
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

mpz_class SHE::decrytpSquash(vector<vector<mpz_class>> text)
{
    //cout << "SQUASH" << endl;
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


mpz_class SHE::decrytpRealSquash(vector<vector<mpz_class>> text)
{
    mpz_class left = 0;
    for(int i = 0; i < NB_KEY; i++) {
        for(int j = 0; j < NB_ELEM; j++) {
            left ^= (text[i][j] & 1) & (sk[i][j]);
        }
    }

   // cout << "FIN LEFT" << endl;

    vector<mpz_class> v{};
    v.resize(NB_KEY);

    //TODO: log2(NB_KEY) + 1 !
    int precision = 4;

    for(int i = 0; i < NB_KEY; i++) {
        v[i] = 0;
        for(int j = 0; j < NB_ELEM; j++) {
            v[i] ^= (sk[i][j] * ((text[i][j] << precision) / d));
        }
    }

   // cout << "FIN V" << endl;

    vector<vector<mpz_class>> columns{};
    columns.resize(precision + 1);
    for(int i = 0; i < precision + 1; i++) {
        columns[i].resize(NB_KEY);
    }
    /**
     * t1 = 0 . 0 1 1 1
     * t2 = 1 . 0 1 0 1
     * sauf que c est reverse
    */

    for(int j = 0; j < NB_KEY; j++) {
        for(int i = 0; i < precision + 1; i++) {
            columns[i][j] = (v[j] >> i) & 1;
        }
    }

  //  cout << "FIN BUILD BIT COLUMN" << endl;

    v = gradeSchoolAddition(columns);

   // cout << "FIN GRADE SCHOOL" << endl;

    mpz_class right = v[precision] ^ v[precision - 1];

   // cout << "RETURN" << endl;
    return left ^ right;
}


//TODO: Mettre ces 3 fonctions en dessous dans un autre fichiers surement ! 
std::vector<int> SHE::next_set(std::vector<int> v, int max)
{

    int i = (int)(v.size() - 1);
    int prev = 0;

    bool flag = true;
    while(flag)
    {

        flag = false;
        for(; i >= 0; i--)
        {
            v[i]++;
            prev = v[i];
            if(v[i] > max)
                v[i] = 0;
            else break;
        }


        i++;
        
        for(; i <= (int)v.size(); i++)
        {
            v[i] = ++prev;
            if(v[i] > max)
            {
                v[i] = 0;
                i--;
                flag = true;
                break;
            }
        }

        if(i < 0) break;
    }
    return v;
}


// n = taille des tuples
// v = ensemble d'éléments à prendre
// sum(xi1, xi2, xi3, ...., xin) pour tout tuples !
mpz_class SHE::polynomial_sym(int n, vector<mpz_class> v)
{
    //Si j'ai moins d'elements que la taille des tuples
    if((unsigned) n > v.size()) {
        // mpz_class res = 1;
        // for(mpz_class &elt : v)
        //     res *= elt;
        // return res;
        return 0;
    }


    //On initialise le tableau
    vector<int> index{};
    unsigned int nb = v.size();
    for(int i = 0; i < n; i++)
        index.push_back(i);

    mpz_class res = 0;

    while(1)
    {
        // for(int i : index) 
        //     cout << i << " ";
        // cout << endl;

        mpz_class prod = 1;
        for(int i : index)
        {
            prod *= v[i];
        }
        res += prod;
        index = next_set(index, nb);

        bool stop = true;
        for(int i : index) {
            if(i != 0) {
                stop = false;
                break;
            }
        }
        if(stop) break;
    }

    return res;
}

vector<mpz_class> SHE::gradeSchoolAddition(vector<vector<mpz_class>> columns)
{
    vector<mpz_class> res{};
    res.resize(columns.size());

    for(unsigned int i = 0; i < columns.size(); i++)
    {
        res[i] = 0;
    }

    for(unsigned int i = 0; i < columns.size(); i++)
    {
        res[i] = 0;
        for(unsigned int j = 0; j < columns[i].size(); j++)
            res[i] ^= columns[i][j];
        int k = 1;
        for(unsigned int j = i + 1; j < columns.size(); j++) {
            mpz_class b = polynomial_sym(1 << k, columns[i]) & 1;
            columns[j].push_back(b);
            k++;
        }

        // for(unsigned int ii = 0; ii < columns.size(); ii++) {
        //     for(unsigned int jj = 0; jj < columns[ii].size(); jj++) {
        //         cout << columns[ii][jj] << " ";
        //     }
        //     cout << endl;
        // }

    }

    return res;
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