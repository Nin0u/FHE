#ifndef SHE_H
#define SHE_H

#include "Polynomial.hpp"

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

        int genKeyCandidate(); // Générateur de candidat

    public:
        SHE(int n, mpz_class max_v);
        virtual ~SHE();
        void genKey(); // Générateur de clé
        virtual mpz_class encrypt(char bit);
        virtual mpz_class decrypt(mpz_class text);

        virtual mpz_class encryptM(std::vector<char> bits);
        virtual std::vector<mpz_class> decryptM(mpz_class);

        virtual mpz_class addCipher(mpz_class c1, mpz_class c2);
        virtual mpz_class mulCipher(mpz_class c1, mpz_class c2);

        bool testPolynomial(int deg, char b);

    friend std::ostream &operator<<(std::ostream &out, const SHE& she);

};

#endif