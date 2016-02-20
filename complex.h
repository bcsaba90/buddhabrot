#ifndef COMPLEX_H
#define COMPLEX_H
#include <iostream>
#include <string>

class Complex {
	public:
		double real;
		double imaginary;
	public:
		Complex();
		Complex(double real, double imaginary);
		double lengthSquared();
		double length();
		double getAngle();
		Complex& operator*=(const Complex& complex);
		Complex& operator/=(const Complex& complex);
		Complex operator*(const Complex& complex);
		Complex operator/(const Complex& complex);
		Complex operator+(const Complex& complex);
		Complex& operator+=(const Complex& complex);
		Complex& operator=(const Complex& complex);
		Complex& power(double n);
		Complex& powerAndAdd(double n, const Complex& complex);
		Complex& integerPower(unsigned int power);
		bool operator==(const Complex& complex) const;
		bool operator<(const Complex& complex) const;
		friend std::ostream& operator<<(std::ostream& out, const Complex& complex);
		Complex addReal(double real);
		double getReal() const;
		double getImaginary() const;
		Complex& set(double real, double imaginary);
		Complex& set(const Complex& complex);
		Complex& parseFromString(const std::string& data);
};
#endif
