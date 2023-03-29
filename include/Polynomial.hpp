#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <vector>
#include <iostream>
#include <time.h>
#include "Fraction.hpp"


class Polynomial{
    private:
        std::vector<Fraction> coeffs;
        int deg;

    public:
        // Constructeurs
        Polynomial();
        Polynomial(int deg);
        Polynomial(std::vector<Fraction> v, int deg);
        Polynomial(const Polynomial &p);
        void generatePolynomial(int min_deg, int max_deg, int max_coeffs);

        Polynomial &operator=(Polynomial p);

        Polynomial mult(int d);

        // Getter
        int getDegree();

        // Surchage d'opérateurs
        Fraction &operator[](int i);
        friend Polynomial operator+(Polynomial & p1, Polynomial &p2);
        friend Polynomial operator-(Polynomial & p1, Polynomial &p2);
        friend Polynomial operator*(Polynomial & p1, Polynomial &p2);

        // Affichage
        friend std::ostream &operator<<(std::ostream &out, const Polynomial &p);

        friend Polynomial euclidianDiv(Polynomial &p1, Polynomial &p2);

        friend Polynomial extendedGCD( Polynomial &p1,  Polynomial &p2);
        // Bezout

};

int puiss(int x,int n);

#endif