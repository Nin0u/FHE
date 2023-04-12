#ifndef SHE_H
#define SHE_H

#include "Polynomial.hpp"

#define MAX_ERROR   5

#define NB_KEY  5
#define NB_ELEM 15

class SHE{
    private: 
        int deg;
        Polynomial polMod;
        gmp_randstate_t state; // Générateur d'entier aléatoire
        mpz_class max_v;

        // Clé publique
        Polynomial v;
        Polynomial w;

        // Clé privée
        mpz_class d;
        mpz_class r;

        mpz_class wi; // Utile pour décrypter

        mpz_class sk [NB_KEY][NB_ELEM];
        mpz_class X  [NB_KEY][NB_ELEM];

        int genKeyCandidate(); // Générateur de candidat
        void splitKey();


    public:
        SHE(int n, mpz_class max_v);
        virtual ~SHE();
        void genKey(); // Générateur de clé
        mpz_class encrypt(char bit);
        mpz_class decrypt(mpz_class text);

        std::vector<std::vector<mpz_class>> expandCT(mpz_class text);
        mpz_class decrytpSquash(std::vector<std::vector<mpz_class>> text);
        mpz_class decrytpRealSquash(std::vector<std::vector<mpz_class>> text);

        mpz_class encryptM(std::vector<char> bits);
        std::vector<mpz_class> decryptM(mpz_class);

        mpz_class addCipher(mpz_class c1, mpz_class c2);
        mpz_class mulCipher(mpz_class c1, mpz_class c2);

        bool testPolynomial(int deg, char b);

        std::vector<int> next_set(std::vector<int> v, int max);
        mpz_class polynomial_sym(int n, std::vector<mpz_class> v);

        std::vector<mpz_class> gradeSchoolAddition(std::vector<std::vector<mpz_class>> column);
    friend std::ostream &operator<<(std::ostream &out, const SHE& she);

};

#endif