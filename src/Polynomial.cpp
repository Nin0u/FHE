#include "Polynomial.hpp"
#include <iostream>
#include <tuple>
#include <cmath>
#include <algorithm>

using namespace std;

// Constructeur
Polynomial::Polynomial() : coeffs{}, deg{0} {}

Polynomial::Polynomial(int deg) : coeffs{}, deg{deg} 
{
    coeffs.reserve(deg + 1);
    for(int i = 0; i <= deg; i++) {
        coeffs[i] = 0;
    }
}

Polynomial::Polynomial(vector<long long int> v, int deg) : coeffs{v}, deg{deg}
{
}

// Constructeur de copie
Polynomial::Polynomial(const Polynomial &p) : coeffs{p.deg + 1}, deg{p.deg}
{
    coeffs.reserve(p.deg + 1);
    for(int i = 0; i <= p.deg; i++)
        coeffs[i] = p.coeffs[i];
}

Polynomial &Polynomial::operator=(Polynomial p) {
    coeffs.reserve(p.deg + 1);
    for(int i = 0; i <= p.deg; i++)
        coeffs[i] = p.coeffs[i];
    this->deg = p.deg;
    return *this;
}

// Génère aléatoirement un polynome
void Polynomial::generatePolynomial(int deg, long long int max_coeffs){
    coeffs.reserve(deg + 1);
    for(int i = 0; i <= deg; i++){
        coeffs.push_back(rand() % (max_coeffs + 1));
        if (rand() % 2) coeffs[i] *= -1;
    }
    if(coeffs[deg] == 0) coeffs[deg] = 1;
    this->deg = deg;
}

// Renvoie le degré
int Polynomial::getDegree() { return deg; }

// [] : retourne le coefficient de X^i
long long int &Polynomial::operator[](int i){
    return coeffs[i];
}

long long int Polynomial::contenu()
{
    long long int pgcd = 0;
    for(int i = 0; i <= deg; i++)
        pgcd = __gcd(pgcd, coeffs[i]);
    return pgcd;
}

// Somme de deux polynômes
Polynomial operator+(Polynomial p1, Polynomial p2){
    Polynomial p3{p2.deg >= p1.deg ? p2.deg : p1.deg};
    if (p2.deg >= p1.deg) {

        int last_not_null = 0;
        for(int i = 0; i <= p1.deg; i++)
        {
            p3[i] = p1[i] + p2[i];
            if(p3[i] != 0) last_not_null = i;
        }
        for(int i = p1.deg + 1; i <= p2.deg; i++) {
            if(p2[i] != 0) last_not_null = i;
            p3[i] = p2[i];
        }
        p3.deg = last_not_null;
    }
    else {
        p3 = p1;
        int last_not_null = 0;
        for(int i = 0; i <= p2.deg; i++)
        {
            p3[i] = p1[i] + p2[i];
            if(p3[i] != 0) last_not_null = i;
        }
        for(int i = p2.deg + 1; i <= p1.deg; i++) {
            if(p1[i] != 0) last_not_null = i;
            p3[i] = p1[i];
        }
        p3.deg = last_not_null;
    }

    return p3;
}


// Soustraction Polynome
Polynomial operator-(Polynomial p1, Polynomial p2){
    Polynomial p3{p2};
    
    for(int i = 0; i <= p2.deg; i++)
        p3[i] *= -1;

    return p1 + p3;
}

// Produit de deux polynômes
// TODO: Un Karatsuba permettrai d'obtimiser ça !
Polynomial operator*(Polynomial p1, Polynomial p2){
    Polynomial p3{p1.deg + p2.deg};
    for(int n = 0; n <= p1.deg + p2.deg; n++) {
        long long int sum = 0;
        for(int k = 0; k <= n; k++) {
            if(k <= p1.deg && (n - k) <= p2.deg && (n - k) >= 0)
                sum += (p1[k] * p2[n - k]);
        }  
        p3.coeffs[n] = sum;
    }
    p3.deg = p1.deg + p2.deg;
    return p3;
}

// Print
ostream &operator<<(std::ostream &out, const Polynomial &p){
    out << p.coeffs[0] << " ";
    if(p.deg == 0) 
    {
        return out;
    }
    out << "+ " << p.coeffs[1] << "X ";
    for(int i = 2; i <= p.deg; i++)
        out << "+ " << p.coeffs[i] << "X^" << i << " ";

    return out;
}

Polynomial Polynomial::mult(long long int d)
{
    Polynomial p{deg};

    for(int i = 0; i <= deg; i++) {
        p.coeffs[i] = d * coeffs[i];
    }
    if(d == 0) p.deg = 0;
    return p;
}

void Polynomial::div(long long int d)
{
    for(int i = 0; i <= deg; i++)
        coeffs[i] /= d;
}

// Euclidian Div par un polynome unitaire !!!!
tuple<long long int, Polynomial, Polynomial> euclidianDiv(Polynomial &p1, Polynomial &p2)
{
    Polynomial q{0};
    Polynomial r{p1};
    int n = p2.deg;
    int m = p1.deg;
    long long d = p2[n];
    int e = m - n + 1;
    while(!(r.coeffs[0] == 0 && r.deg == 0) && r.deg >= n)
    {
       // cout << "R = " << r << endl;
        Polynomial s{r.deg - n};
        s[r.deg - n] = r[r.deg];
        //cout << "S = " << s << endl;
        q = q.mult(d) + s;
        r = r.mult(d) - (s * p2);
        e--;
    }
   // cout << "FIN" << endl;
    d = puiss(d, e);
    q = q.mult(d);
    r = r.mult(d);
    d = puiss(p2[n], m - n + 1);
    //cout << "d == " << p2[n] << " " << m - n + 1 << " " << puiss(p2[n], m - n + 1) << endl;
    //cout << "d = " << d << endl;
    return std::make_tuple(d, q, r);
}


long long int puiss(long long int x, int y)
{
    int res = 1;
    while (y > 0) 
    {
        if (y % 2 == 1)
            res = (res * x);
        y = y >> 1;
        x = (x * x);
    }
    return res;
}

// Bezout
tuple<Polynomial, Polynomial, Polynomial> extendedGCD(Polynomial &p1, Polynomial &p2)
{
    Polynomial r1{p1}, r2{p2}, u1{{1}, 0}, u2{{0}, 0}, v1{{0}, 0}, v2{{1}, 0};
    // int i = 1;
    cout << "DEBUT" << endl;
    while(!( r1.deg == 0))
    {
        Polynomial q, r;
        long long int d;
        std::tie(d, q, r) = euclidianDiv(r1, r2);
        Polynomial rs = r1.mult(d);
        Polynomial us = u1.mult(d);
        Polynomial vs = v1.mult(d);

        r1 = r2;
        u1 = u2;
        v1 = v2;

        r2 = rs - (q * r2);
        u2 = us - (q * u2);
        v2 = vs - (q * v2);

        long long int pgcd = __gcd(r2.contenu(), __gcd(u2.contenu(), v2.contenu()));
        if(pgcd != 0)
        {
            r2.div(pgcd);
            u2.div(pgcd);
            v2.div(pgcd);
        }

    } 

    // cout << r0;
    // cout << s0;
    // cout << t0;
    
    // Polynomial res1 = p1 * s0;
    // Polynomial res2 = p2 * t0;

    // cout << res1 + res2 << endl;
    // return r1;
    return make_tuple(r1, u1, v1);
}
