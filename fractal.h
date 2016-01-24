#ifndef FRACTAL_H
#define FRACTAL_H
#include "fractalParams.h"
#include "vector.h"
#include "bmp.h"
#include "valueProvider.h"
#include <random>

class Fractal {
	FractalParams params;
	ValueProvider* valueProvider;
	inline void increaseValueAt(long** image, const Complex& complex);
	void addToBmp(long** image, Bmp& bmpImage, const Vector& multiplier, double colorMultiplier);
	void deleteImage(long** image);
	long** drawInternal(int index, long long iterationLimit, long long iterationCount, long long minIterationCount);

	public:
		Fractal(ValueProvider* valueProvider, const FractalParams& params);
		void draw();
};

#endif
