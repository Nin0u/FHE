#include "Cipher.hpp"
#include "SHE.hpp"
#include <iostream>

using namespace std;

Cipher::Cipher() : she{nullptr}, value{0} {}

Cipher::Cipher(SHE *she, mpz_class value) : nb_plus{1}, nb_times{1} , she{she}, value{value} {}

Cipher::Cipher(const Cipher & c) : nb_plus{c.nb_plus}, nb_times{c.nb_times}, she{c.she}, value{c.value} {}

unsigned int Cipher::getNbPlus() { return nb_plus; }
unsigned int Cipher::getNbTimes() { return nb_times; }
SHE* Cipher::getSHE() { return she; }
mpz_class Cipher::getValue() { return value; }

Cipher &Cipher::operator=(const Cipher &c) {
    this->nb_plus = c.nb_plus;
    this->nb_times = c.nb_times;
    this->she = c.she;
    this->value = c.value;

    return *this;
}

mpz_class Cipher::getNorm() {
    return she->getNorm(value);
}

void Cipher::setNbTimes(unsigned int nb)
{
    this->nb_times = nb;
}



Cipher operator+(Cipher c1, Cipher c2) {
    // TODO : regarder s'il faut recrypt c1 et c2, le faire et ensuite faire la somme

    Cipher c {c1.she, 0};
    mpz_class d = c1.she->get_d();
    c.value = (c1.value + c2.value) % d;
    if (c.value >=  d / 2) c.value -= d;
    if (c.value < - d / 2) c.value += d; 

    c.nb_plus = c1.nb_plus + c2.nb_plus;
    c.nb_times = (c1.nb_times >= c2.nb_times ? c1.nb_times : c2.nb_times);

    return c;
}

Cipher operator*(Cipher c1, Cipher c2) {
    // TODO : si c1.nb_times + c2.nb_times  max_times : Recrypt value et mettre le nouveau nb_plus =1 et nb_times = 2
    Cipher c {c1.she, 0};
    mpz_class d = c1.she->get_d();
    c.value = (c1.value * c2.value) % d;
    if (c.value >=  d / 2) c.value -= d;
    if (c.value < - d / 2) c.value += d; 

    c.nb_plus = (c1.nb_plus >= c2.nb_plus ? c1.nb_plus : c2.nb_plus);
    c.nb_times = c1.nb_times + c2.nb_times;

    return c;
}

Cipher operator*(Cipher c1, mpz_class c2) {
    // TODO : si c1.nb_times + c2.nb_times  max_times : Recrypt value et mettre le nouveau nb_plus =1 et nb_times = 2
    
    Cipher c {c1.she, 0};
    mpz_class d = c1.she->get_d();
    c.value = (c1.value * c2) % d;
    if (c.value >=  d / 2) c.value -= d;
    if (c.value < - d / 2) c.value += d; 

    c.nb_times = c1.nb_times + 1;

    return c;
}

/** Ramène le chiffré dans l'intervalle [0, d) */
Cipher operator%(Cipher c1, mpz_class d) {
    Cipher c{c1.she, c1.value % d};
    if (c.value < 0) c.value += d;
    c.nb_plus = c1.nb_plus;
    c.nb_times = c1.nb_times;

    return c;
}

ostream &operator<<(ostream &out, const Cipher& c) {
    out << c.value << endl;
    return out;
}