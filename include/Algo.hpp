#ifndef ALGO_HPP
#define ALGO_HPP

#include <gmpxx.h>
#include <vector>
#include <tuple>
#include <fstream>
#include "Polynomial.hpp"
#include "Cipher.hpp"

std::vector<int> next_set(std::vector<int> v, int max);
mpz_class polynomial_sym(int n, std::vector<mpz_class> v);
std::vector<mpz_class> gradeSchoolAddition(std::vector<std::vector<mpz_class>> column);
std::vector<mpz_class> gradeSchoolAddition(std::vector<std::vector<mpz_class>> column, mpz_class d);

// With Cipher 
Cipher polynomial_sym(int n, std::vector<Cipher> v);
std::vector<Cipher> gradeSchoolAddition(std::vector<std::vector<Cipher>> column);
std::vector<Cipher> gradeSchoolAddition(std::vector<std::vector<Cipher>> column, std::ofstream &outfile);

#endif