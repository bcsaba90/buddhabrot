#ifndef COMPLEX_H
#define COMPLEX_H
#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <algorithm>
#include "generalException.h"

class Complex {
	public:
		double real;
		double imaginary;

		
		Complex() {
			this->real = 0.0;
			this->imaginary = 0.0;
		}

		Complex(double real, double imaginary) {
			this->real = real;
			this->imaginary = imaginary;
		}

		inline double lengthSquared() const noexcept {
			return real * real + imaginary * imaginary;
		}

		inline double length() const noexcept {
			return sqrt(lengthSquared());
		}

		inline double getAngle() const noexcept {
			return atan2(imaginary, real);
		}

		inline Complex operator*(const Complex& complex) const noexcept {
			double newReal = real * complex.real - imaginary * complex.imaginary;
			double newImaginary = real * complex.imaginary + imaginary * complex.real;
			return Complex(newReal, newImaginary);
		}

		inline Complex& operator*=(const Complex& complex) noexcept {
			double newReal = real * complex.real - imaginary * complex.imaginary;
			double newImaginary = real * complex.imaginary + imaginary * complex.real;
			this->real = newReal;
			this->imaginary = newImaginary;
			return *this;
		}

		inline Complex& power(double n) noexcept {
			double r = length();
			double theta = getAngle();
			double rPowered = pow(r, n);
			this->real = rPowered * cos(n * theta);
			this->imaginary = rPowered * sin(n * theta);
			return *this;
		}

		inline Complex& powerAndAdd(double n, const Complex& complexToAdd) noexcept {
			double r = length();
			double theta = getAngle();
			double rPowered = pow(r, n);
			this->real = rPowered * cos(n * theta) + complexToAdd.real;
			this->imaginary = rPowered * sin(n * theta) + complexToAdd.imaginary;
			return *this;
		}

		inline Complex& integerPower(unsigned int power) noexcept {
			if (power == 1) {
				return *this;
			}
			Complex result(1,0);
			Complex& z = *this;
			Complex value = z;
			while (power > 0) {
				if (power & 1) {
					result = result * value;
				}
				value *= value;
				power >>= 1;
			}
			this->real = result.real;
			this->imaginary = result.imaginary;
			return *this;
		}

		inline Complex operator/(const Complex& complex) const noexcept {
			double divisor = (complex.real*complex.real + complex.imaginary*complex.imaginary);
			double newReal = (real * complex.real + imaginary * complex.imaginary) / divisor;
			double newImaginary = (imaginary * complex.real - real * complex.imaginary) / divisor;
			return Complex(newReal, newImaginary);
		}

		inline Complex& operator/=(const Complex& complex) noexcept {
			double newReal = real * complex.real - imaginary * complex.imaginary;
			double newImaginary = real * complex.imaginary + imaginary * complex.real;
			this->real = newReal;
			this->imaginary = newImaginary;
			return *this;
		}

		inline Complex operator+(const Complex& complex) const noexcept {
			double newReal = real + complex.real;
			double newImaginary = imaginary + complex.imaginary;
			return Complex(newReal, newImaginary);
		}

		inline Complex& operator+=(const Complex& complex) noexcept {
			real = real + complex.real;
			imaginary = imaginary + complex.imaginary;
			return *this;
		}

		inline Complex& operator=(const Complex& complex) noexcept {
			this->real = complex.real;
			this->imaginary = complex.imaginary;
			return *this;
		}

		inline Complex addReal(double real) noexcept {
			return Complex(this->real + real, this->imaginary);
		}

		inline double getReal() const noexcept {
			return real;
		}

		inline double getImaginary() const noexcept {
			return imaginary;
		}

		inline Complex& set(double real, double imaginary) noexcept {
			this->real = real;
			this->imaginary = imaginary;
			return *this;
		}

		inline Complex& set(const Complex& complex) noexcept {
			this->real = complex.real;
			this->imaginary = complex.imaginary;
			return *this;
		}

		inline bool operator==(const Complex& complex) const noexcept {
			return this->imaginary == complex.imaginary && this->real == complex.real;
		}

		inline bool operator<(const Complex& complex) const noexcept {
			return this->real < complex.real || (this->real == complex.real && this->imaginary < complex.imaginary);
		}
				

		Complex& parseFromString(const std::string& data) throw(GeneralException) {
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

};

#endif
