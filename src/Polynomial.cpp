#include "Polynomial.hpp"
#include <iostream>

using namespace std;

// Constructeur
Polynomial::Polynomial() : coeffs{}, deg{0} {}

Polynomial::Polynomial(int deg) : coeffs{deg + 1}, deg{deg} 
{
    coeffs.reserve(deg + 1);
    for(int i = 0; i <= deg; i++) {
        coeffs[i] = 0;
    }
}

Polynomial::Polynomial(vector<int> v, int deg) : coeffs{v}, deg{deg}
{
}

// Constructeur de copie
Polynomial::Polynomial(const Polynomial &p) : coeffs{p.deg + 1}, deg{p.deg}
{
    coeffs.reserve(p.deg + 1);
    for(int i = 0; i <= p.deg; i++)
        coeffs[i] = p.coeffs[i];
}

// Génère aléatoirement un polynome
void Polynomial::generatePolynomial(int min_deg, int max_deg, int max_coeffs){
    srand(time(NULL));
    int n = (rand() % (max_deg - min_deg + 1)) + min_deg;

    for(int i = 0; i <= n; i++){
        coeffs.push_back(rand() % (max_coeffs + 1));
        if (rand() % 2) coeffs[i] *= -1;
    }
    if(coeffs[n] == 0) coeffs[n] = 1;
    this->deg = n;
}

// Renvoie le degré
int Polynomial::getDegree() { return deg; }

// [] : retourne le coefficient de X^i
int &Polynomial::operator[](int i){
    return coeffs[i];
}

// Somme de deux polynômes
Polynomial operator+(Polynomial &p1, Polynomial &p2){
    Polynomial p3{p2.deg >= p1.deg ? p2.deg : p1.deg};
    if (p2.deg >= p1.deg) {
        p3 = p2;

        int last_not_null = 0;
        for(int i = 0; i <= p1.deg; i++)
        {
            p3[i] += p1[i];
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
            p3[i] += p2[i];
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
Polynomial operator-(Polynomial &p1, Polynomial &p2){
    Polynomial p3{p2};
    
    for(int i = 0; i < p2.deg; i++)
        p3[i] = -p3[i];

    return p1 + p3;
}

// Produit de deux polynômes
// TODO: Un Karatsuba permettrai d'obtimiser ça !
Polynomial operator*(Polynomial &p1, Polynomial &p2){
    Polynomial p3{p1.deg + p2.deg};
    for(int n = 0; n <= p1.deg + p2.deg; n++) {
        int sum = 0;
        for(int k = 0; k <= n; k++) {
            if(k <= p1.deg && (n - k) <= p2.deg && (n - k) >= 0)
                sum += p1[k] * p2[n - k];
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
        out << endl;
        return out;
    }
    out << "+ " << p.coeffs[1] << "X ";
    for(int i = 2; i <= p.deg; i++)
        out << "+ " << p.coeffs[i] << "X^" << i << " ";
    out << endl;

    return out;
}

// Euclidian Div par un polynome unitaire !!!!
Polynomial euclidianDiv(Polynomial &p1, Polynomial &p2)
{
    Polynomial q{0};
    Polynomial r{p1};
    int d = p2.deg;
    int c = p2.coeffs[d];

    int i = 0;

    while(r.deg >= d && i <= 5)
    {
        cout << r ;
        cout << q ;
        Polynomial s{r.deg - d};
        s[r.deg - d] = r[r.deg] / c;
        q = q + s;
        Polynomial res = (p2 * s);
        r = r - s;
        i++;
    }

    cout << q ;
    cout << r;

    return q;
}

// Bezout
Polynomial extendedGCD(const Polynomial &p1, const Polynomial &p2)
{
    Polynomial r0{p1}, r1{p2}, s0{{1}, 0}, s1{{0}, 0}, t0{{0}, 0}, t1{{1}, 0};
    int i = 1;
    while(r1.coeffs[0] != 0 && r1.deg != 0)
    {
        i++;
    } 

    return r1;
}
