#ifndef SHE_H
#define SHE_H

#include "Polynomial.hpp"
#include "Cipher.hpp"
#include "Gen.hpp"

#define MAX_ERROR   5
#define NB_KEY  5
#define NB_ELEM 100

class Cipher;

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
        Cipher ck [NB_KEY][NB_ELEM]; // sk chiffré

        int genKeyCandidate(); // Générateur de candidat
        int genKeyCandidateNew(); // Générateur de candidat
        void splitKey();


    public:
        SHE(int n, mpz_class max_v);
        virtual ~SHE();
        void genKey(int mode); // Générateur de clé

        mpz_class get_d();

        Cipher encrypt(mpz_class bit);
        mpz_class decrypt(Cipher text);

        std::vector<std::vector<Cipher>> expandCT(Cipher text);
        mpz_class decryptSquash(std::vector<std::vector<Cipher>> text);
        mpz_class decryptRealSquash(std::vector<std::vector<Cipher>> text);

        Cipher encryptM(std::vector<char> bits);
        std::vector<mpz_class> decryptM(Cipher c);

        Cipher recrypt(Cipher c);

        bool testPolynomial(int deg, char b);

        Polynomial getE(mpz_class c);
        mpz_class getNorm(mpz_class c);

        mpz_class getRDec(); // A supprimer en cas de déploiement

    friend std::ostream &operator<<(std::ostream &out, const SHE& she);

};

#endif