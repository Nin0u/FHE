#ifndef SHE_H
#define SHE_H

#include "Polynomial.hpp"

#define MAX_ERROR   5

#define NB_KEY  5
#define NB_ELEM 100

class SHE{
    private: 
        int deg;
        Polynomial polMod;
        gmp_randstate_t state; // Générateur d'entier aléatoire
        mpz_class max_v;

        Polynomial v;
        Polynomial w;

        // Clé publique
        mpz_class d;
        mpz_class r;

        // Clé privée
        mpz_class wi;
        int sk [NB_KEY][NB_ELEM];
        
        // Ajout à la clé publique (en plus de d et r)
        mpz_class X  [NB_KEY][NB_ELEM];
        mpz_class ck [NB_KEY][NB_ELEM]; // sk chiffré

        int genKeyCandidate(); // Générateur de candidat
        void splitKey();


    public:
        SHE(int n, mpz_class max_v);
        virtual ~SHE();
        void genKey(); // Générateur de clé
        mpz_class encrypt(mpz_class bit);
        mpz_class decrypt(mpz_class text);

        std::vector<std::vector<mpz_class>> expandCT(mpz_class text);
        mpz_class decrytpSquash(std::vector<std::vector<mpz_class>> text);
        mpz_class decrytpRealSquash(std::vector<std::vector<mpz_class>> text);
        mpz_class decrytpRealSquash2(std::vector<std::vector<mpz_class>> text);

        mpz_class encryptM(std::vector<char> bits);
        std::vector<mpz_class> decryptM(mpz_class);

        mpz_class recrypt(mpz_class text);

        mpz_class addCipher(mpz_class c1, mpz_class c2);
        mpz_class mulCipher(mpz_class c1, mpz_class c2);

        bool testPolynomial(int deg, char b);

    friend std::ostream &operator<<(std::ostream &out, const SHE& she);

};

#endif