#ifndef ALGO_HPP
#define ALGO_HPP

#include <gmpxx.h>
#include <vector>
#include <tuple>
#include "Polynomial.hpp"
#include "Cipher.hpp"

std::vector<int> next_set(std::vector<int> v, int max);
mpz_class polynomial_sym(int n, std::vector<mpz_class> v);
std::vector<mpz_class> gradeSchoolAddition(std::vector<std::vector<mpz_class>> column);
std::vector<mpz_class> gradeSchoolAddition(std::vector<std::vector<mpz_class>> column, mpz_class d);

// With Cipher 
Cipher polynomial_sym(int n, std::vector<Cipher> v);
std::vector<Cipher> gradeSchoolAddition(std::vector<std::vector<Cipher>> column);


mpz_class getDet(const std::vector<std::vector<mpz_class>> matrix);
std::vector<std::vector<mpz_class>> getTranspose(const std::vector<std::vector<mpz_class>> matrix);
std::vector<std::vector<mpz_class>> getCofactor(const std::vector<std::vector<mpz_class>> matrix);
std::vector<std::vector<mpz_class>> getInverse(const std::vector<std::vector<mpz_class>> matrix);
mpz_class getNorm(const std::vector<std::vector<mpz_class>> basis);
mpz_class getRadius(const std::vector<std::vector<mpz_class>> basis);

#endif