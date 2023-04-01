#include "Polynomial.hpp"
#include <iostream>
#include <tuple>

using namespace std;

int main(void) 
{
    srand(time(NULL));
    
    Polynomial p1{4};
    p1.generatePolynomial(5, 10);
    
    Polynomial p2{2};
    p2.generatePolynomial(3, 10);

    cout << p1 << endl;
    cout << p2 << endl;

    cout << p1 + p2 << endl;
    cout << p1 * p2 << endl;

    long long int d;
    Polynomial q, r;

    std::tie(d, q, r) = euclidianDiv(p1, p2);    
    cout << d << " " << q << " " << r << endl;
    cout << p1.mult(d) << endl;
    cout << p2 * q + r << endl;

    Polynomial g, u, v;
    std::tie(g, u, v) = extendedGCD(p1, p2);

    cout << "R = " << g << " U = " << u << " V = " << v << endl;
    cout << u * p1 + v * p2 << endl;
    cout << g << endl;

    return 0;
}