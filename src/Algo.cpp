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

Cipher polynomial_sym(int n, vector<Cipher> v)
{
    //Si j'ai moins d'elements que la taille des tuples
    if((unsigned) n > v.size())
        return Cipher{v[0].getSHE(), 0};

    //On initialise le tableau
    vector<int> index{};
    unsigned int nb = v.size();
    for(int i = 0; i < n; i++)
        index.push_back(i);

    Cipher res = {v[0].getSHE(), 0};

    // Pour chaque tuple, je calcule le produit que j'ajoute à la somme
    while(1)
    {

        //Calcul du produit
        Cipher prod{v[0].getSHE(), 1};
        for(int i : index)
            prod = prod * v[i];

        //Ajout à la somme
        res = res + prod;

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

vector<Cipher> gradeSchoolAddition(vector<vector<Cipher>> columns)
{
    //Initialisation
    vector<Cipher> res{};
    res.resize(columns.size());

    for(unsigned int i = 0; i < columns.size(); i++)
        res[i] = {columns[i][0].getSHE(), 0};

    // Pour chaque colonne
    for(unsigned int i = 0; i < columns.size(); i++)
    {
        //On calcule le Xor
        for(unsigned int j = 0; j < columns[i].size(); j++)
            res[i] = res[i] + columns[i][j];
        int k = 1;

        //Puis grace aux polynome symétrique, on add dans la bonne colonne
        for(unsigned int j = i + 1; j < columns.size(); j++) {
            //cout << i << " " << j << " " << k << endl;
            Cipher b = polynomial_sym(1 << k, columns[i]);
            columns[j].push_back(b);
            k++;
        }
    }
    return res;
}

mpz_class getDet(const std::vector<std::vector<mpz_class>> matrix) {
    size_t dim = matrix[0].size();
    size_t height = matrix.size();
    if(height != dim) {
        throw std::runtime_error("Matrice non carrée");
    }

    if(dim == 0) {
        return 1;
    }

    if(dim == 1) {
        return matrix[0][0];
    }

    if(dim == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }

    mpz_class res = 0;
    int sign = 1;
    for(size_t i = 0; i < dim; i++) {
        std::vector<std::vector<mpz_class>> subVect(dim - 1, std::vector<mpz_class> (dim - 1));
        for(size_t m = 1; m < dim; m++) {
            int z = 0;
            for(size_t n = 0; n < dim; n++) {
                if(n != i) {
                    subVect[m-1][z] = matrix[m][n];
                    z++;
                }
            }
        }

        res = res + sign * matrix[0][i] * getDet(subVect);
        sign = -sign;
    }

    return res;
}

std::vector<std::vector<mpz_class>> getTranspose(const std::vector<std::vector<mpz_class>> matrix) {
    size_t width = matrix[0].size();
    size_t height = matrix.size();
    std::vector<std::vector<mpz_class>> res{width, std::vector<mpz_class>{height}};
    for(size_t i = 0; i < height; i++) {
        for(size_t j = 0; j < width; j++) {
            res[j][i] = matrix[i][j];
        }
    }
    return res;
}

std::vector<std::vector<mpz_class>> getCofactor(const std::vector<std::vector<mpz_class>> matrix) {
    size_t width = matrix[0].size();
    size_t dim = matrix.size();
    if(width != dim) {
        throw std::runtime_error("Matrice non carrée");
    } 

    std::vector<std::vector<mpz_class>> res{dim, std::vector<mpz_class>{dim}};
    std::vector<std::vector<mpz_class>> subVect{dim - 1, std::vector<mpz_class>{dim - 1}};

    for(size_t i = 0; i < dim; i++) {
        for(size_t j = 0; j < dim; j++) {

            int p = 0;
            for(size_t x = 0; x < dim; x++) {
                if(x == i) {
                    continue;
                }
                int q = 0;

                for(size_t y = 0; y < dim; y++) {
                    if(y == j) {
                        continue;
                    }

                    subVect[p][q] = matrix[x][y];
                    q++;
                }
                p++;
            }
            int sign = ((i + j) % 2) ? -1 : 1;
            res[i][j] = sign * getDet(subVect);
        }
    }
    return res;
}

std::vector<std::vector<mpz_class>> getInverse(const std::vector<std::vector<mpz_class>> matrix) {
    size_t dim = matrix.size();
    if(getDet(matrix) == 0) {
        throw std::runtime_error("Déterminant nul");
    } 

    mpz_class d = 1/getDet(matrix);
    std::vector<std::vector<mpz_class>> res{dim, std::vector<mpz_class>{dim}};

    for(size_t i = 0; i < dim; i++) {
        for(size_t j = 0; j < dim; j++) {
            res[i][j] = matrix[i][j]; 
        }
    }

    res = getTranspose(getCofactor(res));

    for(size_t i = 0; i < dim; i++) {
        for(size_t j = 0; j < dim; j++) {
            res[i][j] *= d;
        }
    }

    return res;
}

mpz_class getNorm(const std::vector<std::vector<mpz_class>> basis) {
    return 1; // TODO
}

mpz_class getRadius(const std::vector<std::vector<mpz_class>> basis) {
    return 1 / (2 * getNorm(getTranspose(getInverse(basis))));
}