#ifndef CIPHER_HPP
#define CIPHER_HPP

#include <gmpxx.h>

class SHE;

class Cipher {
    private: 
        unsigned int nb_plus;
        unsigned int nb_times;

        SHE *she;
        mpz_class value;

    public:
        Cipher();
        Cipher(SHE *she, mpz_class value);
        Cipher(const Cipher & c);

        unsigned int getNbPlus();
        unsigned int getNbTimes();
        mpz_class getNorm();
        SHE* getSHE();
        mpz_class getValue();

        Cipher &operator=(const Cipher &c);
        friend Cipher operator+(Cipher c1, Cipher c2);
        friend Cipher operator*(Cipher c1, Cipher c2);
        friend Cipher operator*(Cipher c1, mpz_class c2);
        friend Cipher operator%(Cipher c1, mpz_class d);
        friend std::ostream &operator<<(std::ostream &out, const Cipher& c);
};

#endif