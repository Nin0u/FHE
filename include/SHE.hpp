#ifndef SHE_H
#define SHE_H

#include "Polynomial.hpp"

class SHE{
    private: 
        int deg;
        Polynomial polMod;

        // Clé publique
        Polynomial v;
        Polynomial w;

        // Clé privée
        BigInt d;
        BigInt r;

        BigInt wi; // Utile pour décrypter

        int genKeyCandidate(); // Générateur de candidat

    public:
        SHE(int deg);
        void genKey(); // Générateur de clé

    friend std::ostream &operator<<(std::ostream &out, const SHE& she);

};

#endif