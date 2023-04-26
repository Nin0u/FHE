#ifndef ALGO_HPP
#define ALGO_HPP

#include <gmpxx.h>
#include <vector>
#include <tuple>
#include "Polynomial.hpp"

std::vector<int> next_set(std::vector<int> v, int max);
mpz_class polynomial_sym(int n, std::vector<mpz_class> v);
std::vector<mpz_class> gradeSchoolAddition(std::vector<std::vector<mpz_class>> column);
std::vector<mpz_class> gradeSchoolAddition(std::vector<std::vector<mpz_class>> column, mpz_class d);

//Chinese Remainder Theorem ou Theoreme des Reste Chinois
mpz_class CRT(mpz_class a1, mpz_class p1, mpz_class a2, mpz_class p2);

void CRT_polynomial(Polynomial v, Polynomial X, mpz_class *primes, int i, int deep, Polynomial *rep, mpz_class *r);

std::tuple<Polynomial, mpz_class> invert_Polynomial(Polynomial v, Polynomial X);

#endif