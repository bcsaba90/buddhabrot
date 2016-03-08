#include "complex.h"

std::ostream& operator<<(std::ostream& out, const Complex& complex) {
	out << "(" << complex.real << ", " << "i" << complex.imaginary << ")" << std::endl;
	return out;
}