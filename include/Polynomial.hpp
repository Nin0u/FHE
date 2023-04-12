#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H
#include <vector>
#include <gmpxx.h>

class Polynomial{
    private:
        int deg; // Le degré du polynome.
        std::vector<mpz_class> coeffs; // List des coefficients.

        void updateDeg(); // Fait en sorte que le deg soit cohérent avec le nombre de coeffs

    public:
        // Constructeurs
        Polynomial();
        Polynomial(int deg);
        Polynomial(int deg, std::vector<mpz_class> fill_values);
        Polynomial(int deg, mpz_class max_coeffs, gmp_randstate_t &state);
        Polynomial(const Polynomial &p);

        // Getter
        int getDeg();

        // Surchage d'opérateurs
        mpz_class &operator[](int i);
        Polynomial &operator=(Polynomial p);
        friend Polynomial operator+(Polynomial p1, Polynomial p2);
        friend Polynomial operator-(Polynomial p1, Polynomial p2);
        friend Polynomial operator*(Polynomial p1, Polynomial p2);
        friend Polynomial operator*(Polynomial p1, mpz_class b);
        friend Polynomial operator/(Polynomial p1, mpz_class d);
        friend std::ostream &operator<<(std::ostream &out, const Polynomial &p);

        mpz_class contenu();
        mpz_class eval(mpz_class r);
        mpz_class evalmod(mpz_class r, mpz_class mod);
        int isZero();
        int hasOddCoeff();

        std::tuple<mpz_class, Polynomial, Polynomial> EuclidianDiv(Polynomial Q);
        std::tuple<Polynomial, Polynomial, Polynomial> Bezout(Polynomial Q);

};

#endif