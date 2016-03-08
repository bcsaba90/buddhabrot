#ifndef NUMERIC_HELPER_H
#define NUMERIC_HELPER_H
#include <cmath>
#include <string>

namespace NumericHelper {
	const double DELTA = 0.00001;
	
	inline static double map(double value, double originalRangeMin, double originalRangeMax, double newRangeMin, double newRangeMax) {
		double mappedToUnitRange = (value - originalRangeMin) / (originalRangeMax - originalRangeMin);
		return mappedToUnitRange * (newRangeMax - newRangeMin) + newRangeMin;
	}
	
	inline bool isNumeric(const std::string& string) {
		for (unsigned int i = 0; i < string.size(); ++i) {
			if (string[i] < '0' || string[i] > '9') {
				return false;
			}
		}
		return true;
	}
	
	inline bool equalsTo(double numberToCheck, int with) {
		return fabs(numberToCheck - with) < DELTA;
	}
	
	inline bool isIntegerNumber(double numberToCheck) {
		return fabs(numberToCheck - std::round(numberToCheck)) < DELTA;
	}
}
#endif