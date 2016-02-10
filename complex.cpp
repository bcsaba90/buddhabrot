#include "complex.h"
#include "generalException.h"
#include <cstring>
#include <cmath>
#include <algorithm>

Complex::Complex() {
	this->real = 0.0;
	this->imaginary = 0.0;
}

Complex::Complex(double real, double imaginary) {
	this->real = real;
	this->imaginary = imaginary;
}

double Complex::lengthSquared() {
	return real * real + imaginary * imaginary;
}

double Complex::length() {
	return sqrt(lengthSquared());
}

double Complex::getAngle() {
	return atan2(imaginary, real);
}

Complex Complex::operator*(const Complex& complex) {
	double newReal = real * complex.real - imaginary * complex.imaginary;
	double newImaginary = real * complex.imaginary + imaginary * complex.real;
	return Complex(newReal, newImaginary);
}

Complex& Complex::operator*=(const Complex& complex) {
	double newReal = real * complex.real - imaginary * complex.imaginary;
	double newImaginary = real * complex.imaginary + imaginary * complex.real;
	this->real = newReal;
	this->imaginary = newImaginary;
	return *this;
}

Complex& Complex::power(double n) {
	double r = length();
	double theta = getAngle();
	double rPowered = pow(r, n);
	this->real = rPowered * cos(n * theta);
	this->imaginary = rPowered * sin(n * theta);
	return *this;
}

Complex& Complex::powerAndAdd(double n, const Complex& complexToAdd) {
	double r = length();
	double theta = getAngle();
	double rPowered = pow(r, n);
	this->real = rPowered * cos(n * theta) + complexToAdd.real;
	this->imaginary = rPowered * sin(n * theta) + complexToAdd.imaginary;
	return *this;
}

Complex& Complex::integerPower(int power) {
	Complex result(1,0);
	Complex& z = *this;
	Complex value = z;
	while (power > 0) {
		if (power & 1) {
			result = value * z;
		}
		value *= value;
		power >>= 1;
	}
	this->real = result.real;
	this->imaginary = result.imaginary;
	return *this;
}

Complex Complex::operator/(const Complex& complex) {
	double divisor = (complex.real*complex.real + complex.imaginary*complex.imaginary);
	double newReal = (real * complex.real + imaginary * complex.imaginary) / divisor;
	double newImaginary = (imaginary * complex.real - real * complex.imaginary) / divisor;
	return Complex(newReal, newImaginary);
}

Complex& Complex::operator/=(const Complex& complex) {
	double newReal = real * complex.real - imaginary * complex.imaginary;
	double newImaginary = real * complex.imaginary + imaginary * complex.real;
	this->real = newReal;
	this->imaginary = newImaginary;
	return *this;
}

Complex Complex::operator+(const Complex& complex) {
	double newReal = real + complex.real;
	double newImaginary = imaginary + complex.imaginary;
	return Complex(newReal, newImaginary);
}

Complex& Complex::operator+=(const Complex& complex) {
	real = real + complex.real;
	imaginary = imaginary + complex.imaginary;
	return *this;
}

Complex& Complex::operator=(const Complex& complex) {
	this->real = complex.real;
	this->imaginary = complex.imaginary;
	return *this;
}

Complex Complex::addReal(double real) {
	return Complex(this->real + real, this->imaginary);
}

double Complex::getReal() const {
	return real;
}

double Complex::getImaginary() const {
	return imaginary;
}

Complex& Complex::set(double real, double imaginary) {
	this->real = real;
	this->imaginary = imaginary;
	return *this;
}

Complex& Complex::set(const Complex& complex) {
	this->real = complex.real;
	this->imaginary = complex.imaginary;
	return *this;
}

bool Complex::operator==(const Complex& complex) const {
	return this->imaginary == complex.imaginary && this->real == complex.real;
}

bool Complex::operator<(const Complex& complex) const {
	return this->real < complex.real || (this->real == complex.real && this->imaginary < complex.imaginary);
}
		

Complex& Complex::parseFromString(const std::string& data) {
	std::string localData = data;
	const char* charsToRemove = " \t()";
  for (unsigned int i = 0; i < std::strlen(charsToRemove); ++i) {
    localData.erase(std::remove(localData.begin(), localData.end(), charsToRemove[i]), localData.end());
  }
  size_t separator = localData.find(",");
  if (separator == std::string::npos) {
  	throw new GeneralException("Cannot parse complex data, missing ',' separator");
  }
  this->real = atof(localData.substr(0, separator).c_str());
  this->imaginary = atof(localData.substr(separator + 1, std::string::npos).c_str());
  return *this;
}

std::ostream& operator<<(std::ostream& out, const Complex& complex) {
	out << "(" << complex.real << ", " << "i" << complex.imaginary << ")" << std::endl;
	return out;
}
