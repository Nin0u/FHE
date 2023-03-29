#include "Fraction.hpp"
#include <algorithm>

using namespace std;

Fraction::Fraction(long int n, long int d) : num{n}, den{d}
{
    if(n == 0 && d == 0) {
        num = n;
        den = 1;
        return;
    }
    if(d == 0) {
        cout << n << " " << d << endl;
        throw InvalidDenom{"Erreur"};
    }
    int pgcd_ = Fraction::pgcd(n, d);
    if(num == 0) {
        num = 0;
        den = 1;
        return;
    }
    num /= pgcd_;
    den /= pgcd_;
}

Fraction::Fraction(const Fraction & f) : num{f.num}, den{f.den}
{

}

Fraction::~Fraction() {}

bool operator==(Fraction &f1, Fraction & f2) {
    return f1.num == f2.num && f1.den == f2.den;
}

bool operator==(Fraction &f1, long int f2) {
    return f1.num == f2 && f1.den == 1;
}


bool operator!=(Fraction &f1, long int f2) {
    return !(f1 == f2);
}

int Fraction::pgcd(long int a, long int b)
{
    // if(a < 0) return pgcd(-a, b);   
    // if(b < 0) return pgcd(a, -b); 
    // if(a == 0) return b;
    // if(b == 0) return a;
    // int d = 0;  
    // while(a != b) {
    //     int flag = 0;
    //     if((a & 1) == 0) {
    //         a >>= 1;
    //         flag++;
    //     }
    //     if((b & 1) == 0) {
    //         b >>= 1;
    //         flag++;
    //     }
    //     if(flag == 2) d++;
    //     if(flag == 0) {
    //         if (a < b) b = (b - a);
    //         else a = (a - b);
    //     }
    // }

    // return a << d;
    return std::__gcd(a, b);
}

ostream &operator<<(ostream &out, const Fraction &f)
{
    out << f.num ;
    if(f.den != 1)
        out << "/" << f.den;
    return out;
}

Fraction &Fraction::operator+=(Fraction & f) {
    int pgcd_den = Fraction::pgcd(den, f.den);
    int new_den = den * f.den / pgcd_den;
    int new_num = (num * f.den + f.num * den) / pgcd_den;
    num = new_num;
    den = new_den;

    return *this;
}

Fraction &Fraction::operator-=(Fraction & f) {
    int pgcd_den = Fraction::pgcd(den, f.den);
    int new_den = den * f.den / pgcd_den;
    int new_num = (num * f.den - f.num * den) / pgcd_den;
    num = new_num;
    den = new_den;
    return *this;
}

Fraction operator*(const Fraction &f1, const Fraction &f2)
{
    Fraction f{f1.num * f2.num, f1.den * f2.den};
    return f;
}

Fraction &Fraction::operator+=(Fraction f) {
    int pgcd_den = Fraction::pgcd(den, f.den);
    int new_den = den * f.den / pgcd_den;
    int new_num = (num * f.den + f.num * den) / pgcd_den;
    num = new_num;
    den = new_den;

    return *this;
}


        
Fraction::operator int() {
    return num;
}

Fraction operator+(const Fraction &f1, const Fraction &f2)
{
    int pgcd_den = Fraction::pgcd(f1.den, f2.den);
    int new_den = f1.den * f2.den / pgcd_den;
    int new_num = (f1.num * f2.den + f2.num * f1.den) / pgcd_den;

    return Fraction{new_num, new_den};
}

Fraction operator-(const Fraction &f1, const Fraction &f2)
{
    int pgcd_den = Fraction::pgcd(f1.den, f2.den);
    int new_den = f1.den * f2.den / pgcd_den;
    int new_num = (f1.num * f2.den - f2.num * f1.den) / pgcd_den;

    return Fraction{new_num, new_den};
}

Fraction Fraction::operator++(int i)
{
    Fraction f = Fraction(*this);
    ++(*this);
    return f;
}

Fraction &Fraction::operator++()
{
    num += den;
    int pgcd_ = Fraction::pgcd(num, den);
    num /= pgcd_;
    den /= pgcd_;

    return *this;
}

Fraction &Fraction::operator*=(long int a) {
    num *= a;
    int d = pgcd(num, den);
    num /= d;
    den /= d;
    return *this;
}

Fraction operator/(const Fraction &f1, const Fraction &f2)
{
    Fraction f{f1.num * f2.den, f1.den * f2.num};
    return f;
}

Fraction operator/(const Fraction &f1, const long int a)
{
    Fraction f{f1.num, f1.den * a};
    return f;
}