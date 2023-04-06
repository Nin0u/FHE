#include "Polynomial.hpp"
#include <time.h>
#include <vector>
#include <iostream>
#include <tuple>
using namespace std;

/**
 * Constructeur
 * Génère le polynome nul.
 */
Polynomial::Polynomial() : deg{0}, coeffs{} { coeffs.push_back(0); }

/**
 * Constructeur
 * Génère le polynome nul à deg+1 coefficients.
 * 
 * @param deg Le degre voulu.
 */
Polynomial::Polynomial(int deg) : deg{deg}, coeffs{} 
{
    coeffs.resize(deg + 1);
    for(int i = 0; i <= deg; i++)
        coeffs[i] = 0;
}

/**
 * Constructeur  
 * Génère un polynome avec des coefficients définis.
 * 
 * @param deg Le degre voulu.
 * @param fill_values Le vecteur des coefficients voulus.
 */
Polynomial::Polynomial(int deg, vector<mpz_class> fill_values) : deg{deg}, coeffs{fill_values} {
    if((int)fill_values.size() < (deg + 1))
        this->deg = fill_values.size() - 1;
}

/**
 * Constructeur
 * Génère aléatoirement un polynome 
 * 
 * @param deg Le degré du polynome
 * @param max_coeffs Borne qui définit l'intervalle des coefficients ]-max_coeffs, max_coeffs[
 * @param coeffs_nb_bits Le nombre de bits que contiendra chaque coeff. Valeur par défaut à la longueur de max_coeff. 
 */
Polynomial::Polynomial(int deg, mpz_class max_coeffs, gmp_randstate_t &state) : deg{deg}, coeffs{}
{
    coeffs.resize(deg + 1);
    for(int i = 0; i <= deg; i++){
        mpz_urandomm(coeffs[i].get_mpz_t(), state, max_coeffs.get_mpz_t());
        if (rand() % 2) coeffs[i] = -coeffs[i];
    }
}

/**
 * Constructeur de copie
 * 
 * @param p Le Polynome qu'on veut copier.
 */
Polynomial::Polynomial(const Polynomial &p) : deg{p.deg}, coeffs{p.deg + 1}
{
    coeffs.resize(p.deg + 1);
    for(int i = 0; i <= p.deg; i++) {
        coeffs[i] = p.coeffs[i];
    }
}

/** Update le degré pour qu'il soit cohérent avec le nombre de coeffs */
void Polynomial::updateDeg(){
    deg = coeffs.size()-1;
}

/** Getter : Le dégré du polynome */
int Polynomial::getDeg() { return deg; }

/** Getter : i-ème coefficient */
mpz_class &Polynomial::operator[](int i) {
    return coeffs[i];
}

/** Affectation */
Polynomial &Polynomial::operator=(Polynomial p) {
    coeffs.resize(p.deg + 1);
    for(int i = 0; i <= p.deg; i++)
        coeffs[i] = p.coeffs[i];
    this->deg = p.deg;
    return *this;
}

/** Somme de deux polynomes */
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

/** Difference de deux polynomes */
Polynomial operator-(Polynomial p1, Polynomial p2){
    Polynomial p3{p2 * mpz_class{-1}};
    return p1 + p3;
}

/** Multiplication de deux polynomes */
// TODO: Un Karatsuba permettrai d'obtimiser ça !
Polynomial operator*(Polynomial p1, Polynomial p2){
    Polynomial p3{p1.deg + p2.deg};
    for(int n = 0; n <= p1.deg + p2.deg; n++) {
        mpz_class sum{0};
        for(int k = 0; k <= n; k++) {
            if(k <= p1.deg && (n - k) <= p2.deg && (n - k) >= 0)
                sum = sum + (p1[k] * p2[n - k]);
        }  
        p3.coeffs[n] = sum;
    }
    p3.deg = p1.deg + p2.deg;
    return p3;
}

/** Multipliation par un entier */
Polynomial operator*(Polynomial p1, mpz_class b) {
    Polynomial res{p1};
    for (int i = 0; i<= p1.deg; i++)
        res[i] *= b;

    return res;
}

/** Division par un entier */
Polynomial operator/(Polynomial p1, mpz_class d) {
    if(d == 0) throw domain_error("Division by Zero");
    Polynomial p3{p1};
    for(int i = 0; i <= p1.deg; i++)
        p3[i] /= d;
    return p3;
}

/** Affichage d'un polynome */
ostream &operator<<(std::ostream &out, const Polynomial &p){
    out << p.coeffs[0] << " ";
    if(p.deg == 0) return out;

    out << "+ " << p.coeffs[1] << "X ";
    for(int i = 2; i <= p.deg; i++)
        out << "+ " << p.coeffs[i] << "X^" << i << " ";

    return out;
}

/** Calcul le contenu du polynome */
mpz_class Polynomial::contenu(){
    mpz_class pgcd = {0};
    for(int i = 0; i <= deg; i++)
        mpz_gcd(pgcd.get_mpz_t(), pgcd.get_mpz_t(), coeffs[i].get_mpz_t());
    return pgcd;
}

/** Evalue un polynome en r */
mpz_class Polynomial::eval(mpz_class r, mpz_class mod){
    mpz_class sum{0};
    mpz_class Ri{1};
    for(int i = 0; i <= deg; i++){
        sum += coeffs[i] * Ri;
        sum %= mod;
        Ri *= r;
        Ri %= mod;
    }
    
    return sum;
}

/** Vérifie si c'est le polynome nul */
int Polynomial::isZero(){
    for(int i = 0; i <= deg; i++)
        if(coeffs[i] != 0) return 0;
    return 1;
}

/** Vérifie si le polynome a un coefficient impair */
int Polynomial::hasOddCoeff(){
    for(int i = 0; i <= deg; i++)
        if (coeffs[i] % 2 == 1 || coeffs[i] % 2 == -1) return i;
    return -1;
}
/** 
 * Division Euclidienne par un polynome 
 * Cette division doit etre sur les entiers,
 * On fait la psuedo division de Knuth de la forme d * P = BQ + R
 * 
 * @return Un tuple contenant d, B, R
 */
tuple<mpz_class, Polynomial, Polynomial> Polynomial::EuclidianDiv(Polynomial p){
    if (p.isZero()) throw domain_error("Division by Zero");
    Polynomial q{0};
    Polynomial r{*this};
    int m = deg;
    int n = p.deg;
    mpz_class d = p[n];
    int e = m - n + 1;

    while(!r.isZero() && r.deg >= n){
        Polynomial s{r.deg - n};
        s[r.deg - n] = r[r.deg];
        q = (q * d) + s;
        r = (r * d) - (s * p);
        e--;
    }

    mpz_pow_ui(d.get_mpz_t(), p[n].get_mpz_t(), e);
    q = q * d;
    r = r * d;
    
    mpz_pow_ui(d.get_mpz_t(), p[n].get_mpz_t(), m - n + 1);
    return std::tie(d,q,r);
}

/**
 * Algorithme d'Euclide etendu pour déterminer les coefficients de Bezout
 * 
 * @return Un tuple contenant R,U,V
 */
tuple<Polynomial, Polynomial, Polynomial> Polynomial::Bezout(Polynomial p){
    Polynomial R1{*this};
    Polynomial R2{p};

    Polynomial U1{0, {mpz_class{1}}};
    Polynomial U2{0, {mpz_class{0}}};
    Polynomial V1{0, {mpz_class{0}}};
    Polynomial V2{0, {mpz_class{1}}};

    while(! R2.isZero()){
        mpz_class d;
        Polynomial q,r;
        tie(d,q,r) = R1.EuclidianDiv(R2);
        
        Polynomial rs = R1 * d;
        Polynomial us = U1 * d;
        Polynomial vs = V1 * d;

        R1 = R2;
        U1 = U2;
        V1 = V2;

        R2 = rs - (q * R2);
        U2 = us - (q * U2);
        V2 = vs - (q * V2);

        mpz_class pgcd = gcd(R2.contenu(), gcd(U2.contenu(), V2.contenu()));
        if(pgcd != 0) {
            R2 = R2 / pgcd;
            U2 = U2 / pgcd;
            V2 = V2 / pgcd;
        }
    }

    return std::tie(R1, U1, V1);
}