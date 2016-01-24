#include "fractal.h"
#include "numericHelper.h"
#include <ctime>
#include <string>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <algorithm>

Fractal::Fractal(ValueProvider* valueProvider, const FractalParams& params) {
	this->params = params;
	this->valueProvider = valueProvider;
}

inline void Fractal::increaseValueAt(long** image, const Complex& complex) {
	int x = NumericHelper::map(complex.getReal(), params.minX, params.maxX, 0, params.width);
	int y = NumericHelper::map(complex.getImaginary(), params.minY, params.maxY, 0, params.height);
	if (x >= 0 && x < params.width && y >= 0 && y < params.height) {
		image[y][x]++;
	}
}

long** Fractal::drawInternal(int index, long long iterationLimit, long long iterationCount, long long minIterationCount) {
	valueProvider->reset(index);
	long** image = new long*[params.height];
	for (int i = 0; i < params.height; ++i) {
		image[i] = new long[params.width];
		for (int j = 0; j < params.width; ++j) {
			image[i][j] = 0;
		}
	}
	Complex* route = new Complex[iterationLimit];
	int rangeCout = iterationCount / 1000;
	Complex c;
	Complex z(0,0);
	for (long long i = 0; i < iterationCount; ++i) {
		c = valueProvider->getNextValue(index);
		z.set(0,0);
		long long k = 0;
		while (k < iterationLimit && z.lengthSquared() <  4.0) {
			z *= z;
			z += c;
			route[k].set(z);
			++k;
		}
		if (k < iterationLimit && k > minIterationCount) {
			for (int l = 0; l < k; ++l) {
				increaseValueAt(image, route[l]);
			}
			valueProvider->lastValueSuccess(k);
		}
		if ((i % rangeCout) == 0) {
			std::cout << index << ". " << (double)i / iterationCount * 100.0 << " %" << std::endl;
		}
	}
	delete[] route;
	return image;
}

void Fractal::deleteImage(long** image) {
  for (int l = 0; l < params.height; ++l) {
		delete[] image[l];
	}	
	delete[] image;
}

void Fractal::addToBmp(long** image, Bmp& bmpImage, const Vector& multiplier, double colorMultiplier) {
	long long count = 0;
	long long nonZeroCount = 0;
	for (int i = 0; i < params.height; ++i) {
		for (int j = 0; j < params.width; ++j) {
			count += image[i][j];
			if (image[i][j] != 0) {
				++nonZeroCount;
			}
		}
	}
	count = (count * 2.0 * colorMultiplier) / nonZeroCount;
	if (count == 0) {
		count = 1;
	}
	for (int i = 0; i < params.height; ++i) {
		for (int j = 0; j < params.width; ++j) {
			int color = NumericHelper::map(image[i][j], 0, count, 0, 255);
			bmpImage.addToPixel(j, i, Vector(color, color, color)*multiplier);
		}
	}
}

void Fractal::draw() {
	Bmp bmpImage(params.width, params.height);
	bmpImage.fill(Vector(0,0,0));
	for (int i = 0; i < params.colorStep.size(); ++i) {
		long** color = drawInternal(i, params.colorStep[i].iterationLimit, params.colorStep[i].iterationCount, params.colorStep[i].minIterationCount);
		addToBmp(color, bmpImage, params.colorStep[i].color, params.colorStep[i].colorMultiplier);
		deleteImage(color);
	}
	std::stringstream ss;
	ss << params.fileName << "_" << time(NULL) << ".bmp";
	bmpImage.save(ss.str());
	std::cout << "Saved fractal to file " << ss.str() << std::endl;
}
