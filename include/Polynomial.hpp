#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <vector>
#include <iostream>
#include <time.h>

#include "BigInt.hpp"

class Polynomial{
    private:
        int deg; // Le degré du polynome.
        std::vector<BigInt> coeffs; // List des coefficients.

    public:
        // Constructeurs
        Polynomial();
        Polynomial(int deg);
        Polynomial(int deg, std::vector<BigInt> fill_values);
        Polynomial(int deg, BigInt max_coeffs, int coeffs_nb_bits = -1);
        Polynomial(const Polynomial &p);

        // Getter
        int getDegree();

        // Surchage d'opérateurs
        BigInt &operator[](int i);
        Polynomial &operator=(Polynomial p);
        friend Polynomial operator+(Polynomial p1, Polynomial p2);
        friend Polynomial operator-(Polynomial p1, Polynomial p2);
        friend Polynomial operator*(Polynomial p1, Polynomial p2);
        friend Polynomial operator*(Polynomial p1, BigInt b);
        friend Polynomial operator/(Polynomial p1, BigInt d);
        friend std::ostream &operator<<(std::ostream &out, const Polynomial &p);

        // Calcul le contenu
        BigInt contenu();
        int isZero();
        int hasOddCoeff();

        std::tuple<BigInt, Polynomial, Polynomial> EuclidianDiv(Polynomial Q);
        std::tuple<Polynomial, Polynomial, Polynomial> Bezout(Polynomial Q);

};

#endif