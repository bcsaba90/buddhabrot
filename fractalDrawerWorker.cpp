#include "fractalDrawerWorker.h"
#include "numericHelper.h"
#include "complex.h"
#include <iostream>


FractalDrawerWorker::FractalDrawerWorker(ResultHolder& resultHolder, WorkProvider& workProvider, ProgressIndicator& progressIndicator, ValueProvider& valueProvider, FractalParams& params, unsigned int threadIndex) : resultHolder(resultHolder), workProvider(workProvider), progressIndicator(progressIndicator), valueProvider(valueProvider), params(params) {
	this->threadIndex = threadIndex;
}

FractalDrawerWorker::~FractalDrawerWorker() {
	if (thread->joinable()) {
		thread->join();
	}
	delete thread;
}

void FractalDrawerWorker::start() {
	isRunning = true;
	thread = new std::thread(&FractalDrawerWorker::run, this);
}

void FractalDrawerWorker::waitUnitFinished() {
	if (thread->joinable()) {
		thread->join();
	}
}

void FractalDrawerWorker::increaseValue(long long iterationCount, Complex* route, Complex& lastNumber) {
	resultHolder.addToResult(route, iterationCount);
	valueProvider.lastValueSuccess(lastNumber, iterationCount);
}

void FractalDrawerWorker::increaseCountAndLogProgress(long long& countSinceLastLog) {
	++countSinceLastLog;
	if (countSinceLastLog > 10000L) {
		progressIndicator.addNumberOfSamples(countSinceLastLog);
		countSinceLastLog = 0L;
	}
}

void FractalDrawerWorker::run() {
	Complex* route = resultHolder.getTempResult();
	Complex c;
	Complex z(0,0);
	long long countSinceLastLog = 0;
	while (isRunning) {
		WorkUnit* workUnit = workProvider.getWorkUnit();
		if (workUnit == nullptr) {
			break;
		}
		unsigned int index = workUnit->index;
		long long iterationLimit = workUnit->iterationLimit;
		long long iterationCount = workUnit->iterationCount;
		long long minIterationCount = workUnit->minIterationCount;
		double power = workUnit->power;
		// Note for the future: Ugly code duplication here. The for loops need to be in the ifs for optimal performance, but that means
		// that many code needs duplication, even though only code inside the while loop needs to be different.
		// Could be solved with macros or dynamic linking.
		if (NumericHelper::equalsTo(power, 2)) {
			for (long long i = 0; i < iterationCount && valueProvider.hasMoreValue(); ++i) {
				c = valueProvider.getNextValue(params.colorStep.size(), threadIndex, true);
				z.set(0,0);
				long long k = 0;
				while (k < iterationLimit && z.lengthSquared() <  4.0) {
					double newReal = z.real * z.real - z.imaginary * z.imaginary + c.real;
					double newImaginary = z.real * z.imaginary + z.imaginary * z.real + c.imaginary;
					z.real = newReal;
					z.imaginary = newImaginary;
					route[k].set(z);
					++k;
				}
				if (k < iterationLimit && k > minIterationCount) {
					increaseValue(k, route, c);
					route = resultHolder.getTempResult();
				}
				increaseCountAndLogProgress(countSinceLastLog);
			}
		} else if (NumericHelper::isIntegerNumber(power) && power >= 0) {
			for (long long i = 0; i < iterationCount && valueProvider.hasMoreValue(); ++i) {
				c = valueProvider.getNextValue(params.colorStep.size(), threadIndex, false);
				z.set(0,0);
				long long k = 0;
				while (k < iterationLimit && z.lengthSquared() <  4.0) {
					z.integerPower((int)std::round(power));
					z += c;
					route[k].set(z);
					++k;
				}
				if (k < iterationLimit && k > minIterationCount) {
					increaseValue(k, route, c);
					route = resultHolder.getTempResult();
				}
				increaseCountAndLogProgress(countSinceLastLog);
			}
		} else {
			for (long long i = 0; i < iterationCount && valueProvider.hasMoreValue(); ++i) {
				c = valueProvider.getNextValue(params.colorStep.size(), threadIndex, false);
				z.set(0,0);
				long long k = 0;
				while (k < iterationLimit && z.lengthSquared() <  4.0) {
					z.powerAndAdd(power, c);
					route[k].set(z);
					++k;
				}
				if (k < iterationLimit && k > minIterationCount) {
					increaseValue(k, route, c);
					route = resultHolder.getTempResult();
				}
				increaseCountAndLogProgress(countSinceLastLog);
			}
		}
	}
	std::cout << "Thread " << threadIndex << " finished" << std::endl;
}