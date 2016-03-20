#include "fractal.h"
#include "numericHelper.h"
#include "pixel.h"
#include "resultHolder.h"
#include "workProvider.h"
#include "progressIndicator.h"
#include "fractalDrawerWorker.h"
#include <ctime>
#include <string>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <iomanip>
#include <thread>

const int NUMBER_OF_DIGITS_OF_POWER_IN_FILENAME = 4;
const long long WORK_SIZE = 100000L; 

Fractal::Fractal(ValueProvider* valueProvider, const FractalParams& params) {
	this->params = params;
	this->valueProvider = valueProvider;
}

void Fractal::addToBmp(int** image, Bmp& bmpImage, const Vector& multiplier, double colorMultiplier) {
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
			bmpImage.addToPixel(j, i, Pixel(Vector(color, color, color)*multiplier));
		}
	}
}

void Fractal::draw(double power, unsigned int numberOfThreads) {
	Bmp bmpImage(params.width, params.height);
	bmpImage.fill(Pixel(0,0,0));
	for (unsigned int i = 0; i < params.colorStep.size(); ++i) {
		valueProvider->reset(i);
		ResultHolder resultHolder(params, i);
		WorkProvider workProvider(params, i, WORK_SIZE, power);
		ProgressIndicator progressIndicator(params);
		
		progressIndicator.initialize();
		resultHolder.initialize();
		
		std::vector<FractalDrawerWorker*> workers;
		for (unsigned int j = 0; j < numberOfThreads; ++j) {
			FractalDrawerWorker* fractalDrawerWorker = new FractalDrawerWorker(resultHolder, workProvider, progressIndicator, *valueProvider, params, j);
			fractalDrawerWorker->start();
			workers.push_back(fractalDrawerWorker);
		}

		for (FractalDrawerWorker* worker : workers) {
			worker->waitUnitFinished();
		}
		int** image = resultHolder.getResult();
		addToBmp(image, bmpImage, params.colorStep[i].color, params.colorStep[i].colorMultiplier);
		for (FractalDrawerWorker* worker : workers) {
			delete worker;
		}
	}
	std::stringstream ss;
	ss << params.fileName << "_" <<  std::fixed << std::setprecision(NUMBER_OF_DIGITS_OF_POWER_IN_FILENAME) << power << "_" << time(NULL);
	bmpImage.save(ss.str());
	std::cout << "Saved fractal to file " << ss.str() << std::endl;
}
