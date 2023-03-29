#ifndef FRACTION_HPP
#define FRACTION_HPP

#include <iostream>
#include <string>

class Fraction
{
    private:
        long int num;
        long int den;

        static int pgcd(long int a, long int b);

    public:
        Fraction(long int num, long int den = 1);
        Fraction(const Fraction &f);
        virtual ~Fraction();

        Fraction &operator++();
        Fraction operator++(int);

        Fraction &operator+=(Fraction & f);
        Fraction &operator+=(Fraction f);
        Fraction &operator-=(Fraction & f);
        Fraction &operator*=(long int a);

        friend bool operator==(Fraction &f1, Fraction & f2);
        friend bool operator==(Fraction &f1, long int f2);
        friend bool operator!=(Fraction &f1, long int f2);

        explicit operator int();

        friend std::ostream &operator<<(std::ostream &out, const Fraction &f);
        friend Fraction operator+(const Fraction &f1, const Fraction &f2);
        friend Fraction operator-(const Fraction &f1, const Fraction &f2);
        friend Fraction operator*(const Fraction &f1, const Fraction &f2);
        friend Fraction operator/(const Fraction &f1, const Fraction &f2);
        friend Fraction operator/(const Fraction &f1, const long int a);

};

class InvalidDenom {
    private:
        std::string mess;

    public:
        InvalidDenom(std::string m) : mess{m} {}
        ~InvalidDenom() {}
};

#endif