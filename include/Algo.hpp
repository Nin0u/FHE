#ifndef ALGO_HPP
#define ALGO_HPP

#include <gmpxx.h>
#include <vector>


std::vector<int> next_set(std::vector<int> v, int max);
mpz_class polynomial_sym(int n, std::vector<mpz_class> v);
std::vector<mpz_class> gradeSchoolAddition(std::vector<std::vector<mpz_class>> column);

#endif