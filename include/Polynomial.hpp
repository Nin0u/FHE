#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H
#include <vector>
#include <gmpxx.h>

class Polynomial{
    private:
        int deg; // Le degré du polynome.
        std::vector<mpz_class> coeffs; // List des coefficients.

    public:
        // Constructeurs
        Polynomial();
        Polynomial(int deg);
        Polynomial(int deg, std::vector<mpz_class> fill_values);
        Polynomial(int deg, mpz_class max_coeffs, int coeffs_nb_bits = -1);
        Polynomial(const Polynomial &p);

        // Getter
        int getDegree();

        // Surchage d'opérateurs
        mpz_class &operator[](int i);
        Polynomial &operator=(Polynomial p);
        friend Polynomial operator+(Polynomial p1, Polynomial p2);
        friend Polynomial operator-(Polynomial p1, Polynomial p2);
        friend Polynomial operator*(Polynomial p1, Polynomial p2);
        friend Polynomial operator*(Polynomial p1, mpz_class b);
        friend Polynomial operator/(Polynomial p1, mpz_class d);
        friend std::ostream &operator<<(std::ostream &out, const Polynomial &p);

        // Calcul le contenu
        mpz_class contenu();
        int isZero();
        int hasOddCoeff();

        std::tuple<mpz_class, Polynomial, Polynomial> EuclidianDiv(Polynomial Q);
        std::tuple<Polynomial, Polynomial, Polynomial> Bezout(Polynomial Q);

};

#endif