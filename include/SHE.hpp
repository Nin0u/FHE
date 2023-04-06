#ifndef SHE_H
#define SHE_H

#include "Polynomial.hpp"

class SHE{
    private: 
        int deg;
        Polynomial polMod;
        gmp_randstate_t state; // Générateur d'entier aléatoire

        // Clé publique
        Polynomial v;
        Polynomial w;

        // Clé privée
        mpz_class d;
        mpz_class r;

        mpz_class wi; // Utile pour décrypter

        int genKeyCandidate(); // Générateur de candidat

    public:
        SHE(int deg);
        ~SHE();
        void genKey(); // Générateur de clé
        mpz_class encrypt(char bit);
        mpz_class decrypt(mpz_class text);

    friend std::ostream &operator<<(std::ostream &out, const SHE& she);

};

#endif