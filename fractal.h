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
	inline void increaseValueAt(int** image, const Complex& complex);
	void addToBmp(int** image, Bmp& bmpImage, const Vector& multiplier, double colorMultiplier);
	void deleteImage(int** image);
	int** drawInternal(unsigned int index, long long iterationLimit, long long iterationCount, long long minIterationCount, double power);
	void increaseValueIfNeeded(long long iterationCount, long long minIterationCount, long long iterationLimit, int** image, Complex* route);
	void logProgress(long long i, int index, int iterationCount, int rangeCout);

	public:
		Fractal(ValueProvider* valueProvider, const FractalParams& params);
		void draw(double power, unsigned int numberOfThreads);
};

#endif
