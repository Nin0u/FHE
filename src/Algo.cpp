#include "Algo.hpp"

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
            prod *= v[i];
        }

        //Ajout à la somme
        res += prod;

        //Cherche le prochain tuple et si = (0, ... , 0) on s'arrete
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