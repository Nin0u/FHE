#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <vector>
#include <iostream>
#include <time.h>

class Polynomial{
    private:
        std::vector<long long int> coeffs;
        int deg;

    public:
        // Constructeurs
        Polynomial();
        Polynomial(int deg);
        Polynomial(std::vector<long long int> v, int deg);
        Polynomial(const Polynomial &p);
        void generatePolynomial(int deg, long long int max_coeffs);

        Polynomial &operator=(Polynomial p);

        void div(long long int d);
        Polynomial mult(long long int d);

        long long int contenu();

        // Getter
        int getDegree();

        // Surchage d'opérateurs
        long long int &operator[](int i);
        friend Polynomial operator+(Polynomial p1, Polynomial p2);
        friend Polynomial operator-(Polynomial p1, Polynomial p2);
        friend Polynomial operator*(Polynomial p1, Polynomial p2);

        // Affichage
        friend std::ostream &operator<<(std::ostream &out, const Polynomial &p);

        friend std::tuple<long long int, Polynomial, Polynomial> euclidianDiv(Polynomial &p1, Polynomial &p2);

        friend std::tuple<Polynomial, Polynomial, Polynomial> extendedGCD( Polynomial &p1,  Polynomial &p2);
        // Bezout

};

long long int puiss(long long int x,int n);

#endif