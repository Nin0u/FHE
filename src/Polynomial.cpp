#include "Polynomial.hpp"
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
    coeffs.reserve(deg + 1);
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
Polynomial::Polynomial(int deg, vector<BigInt> fill_values) : deg{deg}, coeffs{fill_values} {
    if(fill_values.size() < deg + 1)
        this->deg = fill_values.size() - 1;
}

/** Fonction qui aide à générer des BigInt */
void rd(uint8_t * dst, int n){
    for(int i = 1; i < n; i++) dst[i] = (uint8_t)(rand());
}

/**
 * Constructeur
 * Génère aléatoirement un polynome 
 * 
 * @param deg Le degré du polynome
 * @param max_coeffs Borne qui définit l'intervalle des coefficients [-max_coeffs, max_coeffs]
 * @param coeffs_nb_bits Le nombre de bits que contiendra chaque coeff. Valeur par défaut à la longueur de max_coeff. 
 */
Polynomial::Polynomial(int deg, BigInt max_coeffs, int coeffs_nb_bits) : deg{deg}, coeffs{}
{
    int c = coeffs_nb_bits;
    if(coeffs_nb_bits == -1) c = max_coeffs.bitlength();

    srand(time(NULL));
    coeffs.reserve(deg + 1);
    for(int i = 0; i <= deg; i++){
        coeffs[i] = (BigInt::rand_bits(rand() % c, rd)) % BigInt{max_coeffs + 1};
        if (rand() % 2) coeffs[i] *= BigInt{-1};
    }
}

/**
 * Constructeur de copie
 * 
 * @param p Le Polynome qu'on veut copier.
 */
Polynomial::Polynomial(const Polynomial &p) : deg{p.deg}, coeffs{p.deg + 1}
{
    coeffs.reserve(p.deg + 1);
    for(int i = 0; i <= p.deg; i++)
        coeffs[i] = p.coeffs[i];
}

/** Getter : Le dégré du polynome */
int Polynomial::getDegree() { return deg; }

/** Getter : i-ème coefficient */
BigInt &Polynomial::operator[](int i) {
    return coeffs[i];
}

/** Affectation */
Polynomial &Polynomial::operator=(Polynomial p) {
    coeffs.reserve(p.deg + 1);
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
    Polynomial p3{p2 * BigInt{-1}};
    return p1 + p3;
}

/** Multiplication de deux polynomes */
// TODO: Un Karatsuba permettrai d'obtimiser ça !
Polynomial operator*(Polynomial p1, Polynomial p2){
    Polynomial p3{p1.deg + p2.deg};
    for(int n = 0; n <= p1.deg + p2.deg; n++) {
        BigInt sum{0};
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
Polynomial operator*(Polynomial p1, BigInt b) {
    Polynomial res{p1};
    for (int i = 0; i<= p1.deg; i++)
        res[i] *= b;
    if (b == 0) res.deg = 0;
    return res;
}

/** Division par un entier */
Polynomial operator/(Polynomial p1, BigInt d) {
    if(d == 0) throw domain_error("Division by Zero");
    Polynomial p3{p1};
    for(int i = 0; i <= p1.deg; i++)
        p3[i] /= d;

    return p3;
}

/** Affichage d'un polynome */
ostream &operator<<(std::ostream &out, const Polynomial &p){
    p.coeffs[0].write(out) << " ";
    if(p.deg == 0) return out;

    out << "+ ";
    p.coeffs[1].write(out) << "X ";
    for(int i = 2; i <= p.deg; i++){
        out << "+ ";
        p.coeffs[i].write(out) << "X^" << i << " ";
    }

    return out;
}

/** Calcul le contenu du polynome */
BigInt Polynomial::contenu(){
    BigInt pgcd{0};
    for(int i = 0; i <= deg; i++)
        pgcd = BigInt::gcd(pgcd, coeffs[i]);
    return pgcd;
}

/** Vérifie si c'est le polynome nul */
int Polynomial::isZero(){
    for(int i = 0; i <= deg; i++)
        if(coeffs[i] != BigInt{0}) return 0;
    return 1;
}

/** Vérifie si le polynome a un coefficient impair */
int Polynomial::hasOddCoeff(){
    for(int i = 0; i <= deg; i++)
        if (coeffs[i] % BigInt{2} == BigInt{1}) return i;
    return -1;
}
/** 
 * Division Euclidienne par un polynome 
 * Cette division doit etre sur les entiers,
 * On fait la psuedo division de Knuth de la forme d * P = BQ + R
 * 
 * @return Un tuple contenant d, B, R
 */
tuple<BigInt, Polynomial, Polynomial> Polynomial::EuclidianDiv(Polynomial p){
    if (p.isZero()) throw domain_error("Division by Zero");
    Polynomial q{0};
    Polynomial r{*this};
    int m = deg;
    int n = p.deg;
    BigInt d = p[n];
    int e = m - n + 1;

    while(!r.isZero() && r.deg >= n){
        Polynomial s{r.deg - n};
        s[r.deg - n] = r[r.deg];
        q = (q * d) + s;
        r = (r * d) - (s * p);
        e--;
    }

    d = d.pow(e);
    q = q * d;
    r = r * d;
    d = p[n].pow(m - n + 1);

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
    vector<BigInt> zero{};
    zero.push_back(BigInt{0});
    vector<BigInt> one{BigInt{1}};
    one.push_back(1);
    Polynomial U1{0, one};
    Polynomial U2{0, zero};
    Polynomial V1{0, zero};
    Polynomial V2{0, one};

    while(! R2.isZero()){
        BigInt d;
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

        BigInt pgcd = BigInt::gcd(R2.contenu(), BigInt::gcd(U2.contenu(), V2.contenu()));
        if(pgcd != 0) {
            R2 = R2 / pgcd;
            U2 = U2 / pgcd;
            V2 = V2 / pgcd;
        }
    }

    return std::tie(R1, U1, V1);
}