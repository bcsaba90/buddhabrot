
namespace NumericHelper {
	inline static double map(double value, double originalRangeMin, double originalRangeMax, double newRangeMin, double newRangeMax) {
		double mappedToUnitRange = (value - originalRangeMin) / (originalRangeMax - originalRangeMin);
		return mappedToUnitRange * (newRangeMax - newRangeMin) + newRangeMin;
	}
}
