#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <vector>
#include <iostream>
#include <time.h>


class Polynomial{
    private:
        std::vector<int> coeffs;

    public:
        // Constructeurs
        Polynomial();
        Polynomial(const Polynomial &p);
        void generatePolynomial(int min_deg, int max_deg, int max_coeffs);

        // Getter
        int getDegree();

        // Surchage d'opérateurs
        int &operator[](int i);
        friend Polynomial operator+(Polynomial & p1, Polynomial &p2);
        friend Polynomial operator*(Polynomial & p1, Polynomial &p2);

        // Affichage
        friend std::ostream &operator<<(std::ostream &out, const Polynomial &p);

        // Bezout

};

#endif