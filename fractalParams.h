#ifndef FRACTAL_PARAMS_H
#define FRACTAL_PARAMS_H
#include <string>
#include "complex.h"
#include "vector.h"
#include <string>
#include <vector>

struct ColorStep {
	Vector color;
	long long iterationLimit;
	double colorMultiplier;
	long long iterationCount;
	long long minIterationCount;
};

struct FractalParams {
	int width;
	int height;
	std::vector<ColorStep> colorStep;
	double minY, maxY;
	double minX, maxX;
	double randomChance;
	double startPower, endPower, increment;
	std::string fileName;
};
#endif
