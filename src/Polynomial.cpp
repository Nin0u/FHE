#include "Polynomial.hpp"
using namespace std;

// Constructeur
Polynomial::Polynomial() : coeffs{} {}

// Constructeur de copie
Polynomial::Polynomial(const Polynomial &p) : coeffs{(int)(p.coeffs.size())}
{
    for(long unsigned int i = 0; i < p.coeffs.size(); i++)
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
}

// Renvoie le degré
int Polynomial::getDegree() { return coeffs.size() - 1; }

// [] : retourne le coefficient de X^i
int &Polynomial::operator[](int i){
    return coeffs[i];
}

// Somme de deux polynômes
Polynomial operator+(Polynomial &p1, Polynomial &p2){
    Polynomial p3{};
    if (p2.coeffs.size() >= p1.coeffs.size()) {
        p3 = p2;
        for(long unsigned int i = 0; i < p1.coeffs.size(); i++)
            p3[i] += p1[i];
    }
    else {
        p3 = p1;
        for(long unsigned int i = 0; i < p2.coeffs.size(); i++)
            p3[i] += p2[i];
    }

    return p3;
}

// Produit de deux polynômes
Polynomial operator*(Polynomial &p1, Polynomial &p2){
    Polynomial p3{};
    for (long unsigned int i = 0; i <= p1.coeffs.size(); i++){
        Polynomial p4{};
        for(long unsigned int j = 0; j <= p2.coeffs.size(); j++)
            p4[i+j] = p1[i] * p2[j];
        p3 = p3 + p4;
    }

    return p3;
}

// Print
ostream &operator<<(std::ostream &out, const Polynomial &p){
    out << p.coeffs[0] << " ";
    out << "+ " << p.coeffs[1] << "X ";
    for(long unsigned int i = 2; i < p.coeffs.size(); i++)
        out << "+ " << p.coeffs[i] << "X^" << i << " ";
    out << endl;

    return out;
}


int main(void){
    Polynomial p1{};
    p1.generatePolynomial(2,2,10);
    cout << "P1 = " << p1 << endl;


    Polynomial p2{};
    p2.generatePolynomial(3,5,5);
    cout << "P2 = " << p2 << endl;

    cout << "P1 + P2 = " << p1 + p2 << endl;

    return 0;
}