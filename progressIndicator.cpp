#include "progressIndicator.h"
#include <iomanip>
#include <iostream>

ProgressIndicator::ProgressIndicator(FractalParams& fractalParams) : count(0L), params(fractalParams) {

}

ProgressIndicator::~ProgressIndicator() {
	stopThread();
	delete thread;
}


void ProgressIndicator::stopThread() {
	if (thread->joinable()) {
		isRunning = false;
		writeCondition.notify_all();
		thread->join();
	}
}

void ProgressIndicator::initialize() {
	isRunning = true;
	thread = new std::thread(&ProgressIndicator::run, this);
}

void ProgressIndicator::addNumberOfSamples(long long numberOfSamples) {
	count.fetch_add(numberOfSamples);
	writeCondition.notify_all();
}

void ProgressIndicator::run() {
	std::unique_lock<std::mutex> lock(writeMutex);
	while (isRunning) {
		writeCondition.wait(lock);
		long long currentIterationCount = count.load();
		long long allIterationCount = params.colorStep[currentColorStep].iterationCount;
		double newPercent = (double)currentIterationCount / allIterationCount;
		if (newPercent - lastPercent >= 0.001) {
			std::cout << currentColorStep << ". " << std::fixed << std::setprecision(1) << newPercent * 100.0 << " %" << std::endl;
			lastPercent = newPercent;
		}
	}
}

void ProgressIndicator::increaseColorStep() {
	++currentColorStep;
	count.store(0L);
	lastPercent = 0.0;
}

void ProgressIndicator::markThreadFinished(unsigned int threadIndex) {
	std::cout << "Thread " << threadIndex << " finished" << std::endl;
}
