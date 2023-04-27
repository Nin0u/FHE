#include "Algo.hpp"
#include <iostream>
#include <thread>

using namespace std;

// Cette fonction nous renvoie le prochain tuple, utile pour les polynome symétrique
std::vector<int> next_set(std::vector<int> v, int max)
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
        for(; i < (int)v.size(); i++)
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
mpz_class polynomial_sym(int n, vector<mpz_class> v)
{
    //Si j'ai moins d'elements que la taille des tuples
    if((unsigned) n > v.size()) {
        return 0;
    }


    //On initialise le tableau
    vector<int> index{};
    unsigned int nb = v.size();
    for(int i = 0; i < n; i++)
        index.push_back(i);

    mpz_class res = 0;

    // Pour chaque tuple, je calcule le produit que j'ajoute à la somme
    while(1)
    {

        //Calcul du produit
        mpz_class prod = 1;
        for(int i : index)
        {
            prod = prod * v[i];
        }

        //Ajout à la somme
        res += prod;

        //Cherche le prochain tuple et si = (0, ... , 0) on s'arrete
        index = next_set(index, nb - 1);

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

// Algorithme su Grade School Addition utilisant les polynomes symétrique
// Version bits !
// Principe :
// On calcule le Xor de la colonne la plus à gauche, puis pour savoir si cette colonne va avoir
// des retenus pour d'autres, on évalue avec le polynome symétrique, et on met dans les bonnes colonnes les bits
vector<mpz_class> gradeSchoolAddition(vector<vector<mpz_class>> columns)
{
    //Initialisation
    vector<mpz_class> res{};
    res.resize(columns.size());

    for(unsigned int i = 0; i < columns.size(); i++)
    {
        res[i] = 0;
    }

    // Pour chaque colonne
    for(unsigned int i = 0; i < columns.size(); i++)
    {
        res[i] = 0;
        //On calcule le Xor
        for(unsigned int j = 0; j < columns[i].size(); j++)
            res[i] ^= columns[i][j];
        int k = 1;

        //Puis grace aux polynome symétrique, on add dans la bonne colonne
        for(unsigned int j = i + 1; j < columns.size(); j++) {
            //cout << i << " " << j << " " << k << endl;
            mpz_class b = polynomial_sym(1 << k, columns[i]) & 1;
            columns[j].push_back(b);
            k++;
        }
    }
    return res;
}

vector<mpz_class> gradeSchoolAddition(std::vector<std::vector<mpz_class>> columns, mpz_class d)
{
    //Initialisation
    vector<mpz_class> res{};
    res.resize(columns.size());

    for(unsigned int i = 0; i < columns.size(); i++)
    {
        res[i] = 0;
    }

    // Pour chaque colonne
    for(unsigned int i = 0; i < columns.size(); i++)
    {
        res[i] = 0;
        //On calcule le Xor
        for(unsigned int j = 0; j < columns[i].size(); j++) {
            res[i] += columns[i][j];
            res[i] %= d;
            if(res[i] >= d / 2) res[i] -= d;
            if(res[i] < -d / 2) res[i] += d;
            //if(res[i] < 0) res[i] += d;
        }

        int k = 1;

        //Puis grace aux polynome symétrique, on add dans la bonne colonne
        for(unsigned int j = i + 1; j < columns.size(); j++) {
            mpz_class b = polynomial_sym(1 << k, columns[i]);
            b = b % d;
            if(b >= d / 2) b -= d;
            if(b < -d / 2) b += d;
            columns[j].push_back(b);
            k++;
        }
    }
    return res;
}

mpz_class CRT(mpz_class a1, mpz_class p1, mpz_class a2, mpz_class p2)
{
    mpz_class g, u1, u2;
    mpz_gcdext(g.get_mpz_t(), u1.get_mpz_t() ,u2.get_mpz_t(), p1.get_mpz_t(), p2.get_mpz_t());
    
    mpz_class new_mod = p1 * p2;

    return (a2 * u1 * p1 + a1 * u2 * p2) % new_mod;
}

void CRT_polynomial(Polynomial v, Polynomial X, mpz_class *primes, int i, int deep, Polynomial *rep, mpz_class *r, mpz_class *mod)
{
    if(deep == 0)
    {
        mpz_class prime = primes[i];
        cout << i << " : " << prime << endl;
        v.reduce(prime);
        Polynomial G,U,V;
        tie(G,U,V) = X.Bezout(v, prime);
        // V est le polynome recherché mais il est mod prime
        Polynomial a = v * V + U * X;
        a.reduce(prime);

        cout << "Bezout : " << a << " == " << G << endl;

        mpz_class d = G[0];
        *rep = V;
        *r = d;
        *mod = prime;
        return;
    }    


    Polynomial w1, w2;
    mpz_class d1, d2;
    mpz_class mod1, mod2;
    thread t1{CRT_polynomial, v, X, primes, i + 1, deep - 1, &w1, &d1, &mod1};
    thread t2{CRT_polynomial, v, X, primes, i + (1 << deep), deep - 1, &w2, &d2, &mod2};

    t1.join();
    t2.join();

    //cout << "# " << deep << " " << i << " " << mod1 << " " << mod2 << endl;

    mpz_class d = CRT(d1, mod1, d2, mod2);
   // if(d < 0) d += (mod1 * mod2);
   // if(d1 < 0) d1 += mod1;
   // if(d2 < 0) d2 += mod2;

   // cout << "# " << deep << " " << i << " " << d % mod1 << " == " << d1 << " " << d % mod2 << " == " << d2 << endl;

    Polynomial w{X.getDeg() - 1};
    for(int j = 0; j <= X.getDeg() - 1; j++) {
        w[j] = CRT(w1[j], mod1, w2[j], mod2);
    }

    *rep = w;
    *r = d;
    *mod = mod1 * mod2;
    return;
}


tuple<Polynomial, mpz_class> invert_Polynomial(Polynomial v, Polynomial X)
{
    int deep = 1;

    mpz_class primes[(1 << (deep + 1)) - 1];
    mpz_class a = {1};
    a <<= 30;

    mpz_class prime;
    for(int i = 0; i < (1 << (deep + 1)) - 1; i++) {
        mpz_nextprime (prime.get_mpz_t(), a.get_mpz_t());
        a = prime;
        primes[i] = a;
    }

    Polynomial w{X.getDeg() - 1};
    mpz_class d;
    mpz_class mod;

    CRT_polynomial(v, X, primes, 0, deep, &w, &d, &mod);

    return tie(w, d);
}

