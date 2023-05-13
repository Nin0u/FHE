#ifndef GEN_HPP
#define GEN_HPP

#include "Polynomial.hpp"
#include <gmpxx.h>

//Chinese Remainder Theorem ou Theoreme des Reste Chinois
mpz_class get_di(Polynomial v, mpz_class prime, int deg);
mpz_class CRT(mpz_class a1, mpz_class p1, mpz_class a2, mpz_class p2);
void CRT_polynomial(Polynomial v, Polynomial X, mpz_class *primes, int i, int deep, Polynomial *rep, mpz_class *r, mpz_class *mod);
std::tuple<Polynomial, mpz_class> invert_Polynomial(Polynomial v, Polynomial X, int max_thread, int max_prime);

#endif