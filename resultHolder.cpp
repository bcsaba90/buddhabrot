#include "resultHolder.h"
#include "numericHelper.h"
#include <assert.h>
#include <algorithm>
 
const unsigned int NUMBER_OF_TEMP_ARRAY_TO_INITIALIZE_ON_CREATION = 200;
const unsigned int MAX_TIME_TO_WAIT_TO_RECHECK_TASK_LIST = 2000;
 
ResultHolder::ResultHolder(FractalParams& fractalParams, unsigned int colorStepIndex) : params(fractalParams) {
	 this->iterationLimit = fractalParams.colorStep[colorStepIndex].iterationLimit;
	 for (unsigned int i = 0; i < NUMBER_OF_TEMP_ARRAY_TO_INITIALIZE_ON_CREATION; ++i) {
		 Complex* space = new Complex[iterationLimit];
		 freeSpaces.push_back(space);
		 ownedSpaces.push_back(space);
	 }
	 isRunning = false;
	 result = new int*[params.height];
	 for (int i = 0; i < params.height; ++i) {
		 result[i] = new int[params.width];
		 for (int j = 0; j < params.width; ++j) {
		 	 result[i][j] = 0;
		 }
	 }
	 isRunning = false;
}
 
ResultHolder::~ResultHolder() {
	stopThread();
	delete thread;
	for (auto tempSpaceToFree : ownedSpaces) {
		delete[] tempSpaceToFree;
	}
  for (int i = 0; i < params.height; ++i) {
		delete[] result[i];
	}
	delete[] result;
	std::cout << "Deleted resultHolder" << std::endl;
}

void ResultHolder::initialize() {
	isRunning = true;
	thread = new std::thread(&ResultHolder::run, this);
}

void ResultHolder::run() {
	while (isRunning) {
		ResultTempElement resultToProcess = getResultToProcess();
		
		for (int l = 0; l < resultToProcess.size; ++l) {
			Complex& complex = resultToProcess.result[l];
			int x = NumericHelper::map(complex.getReal(), params.minX, params.maxX, 0, params.width);
			int y = NumericHelper::map(complex.getImaginary(), params.minY, params.maxY, 0, params.height);
			if (x >= 0 && x < params.width && y >= 0 && y < params.height) {
				++result[y][x];
			}
		}
		
		freeSpacesAccessMutex.lock();
		freeSpaces.push_front(resultToProcess.result);
		freeSpacesAccessMutex.unlock();
	}
}

Complex* ResultHolder::getTempResult() {
	Complex* result = nullptr;
	freeSpacesAccessMutex.lock();
	if (freeSpaces.size() > 0) {
		result = freeSpaces.front();
		freeSpaces.pop_front();
	} else {
		result = new Complex[iterationLimit];
		ownedSpaces.push_back(result);
	}
	freeSpacesAccessMutex.unlock();
	return result;
}

void ResultHolder::addToResult(Complex* listToAdd, long long size) {
	auto iteratorToOwnedSpace = std::find(ownedSpaces.begin(), ownedSpaces.end(), listToAdd);
	assert(iteratorToOwnedSpace != ownedSpaces.end());
	usedSpacesAccessMutex.lock();
	usedSpaces.push_back(ResultTempElement(listToAdd, size));
	taskAddedCondition.notify_all();
	usedSpacesAccessMutex.unlock();
}

ResultTempElement ResultHolder::getResultToProcess() {
	std::unique_lock<std::mutex> lock(taskAddedMutex);
	bool hasResult = false;
	ResultTempElement result;
	while (isRunning && !hasResult) {
		if (usedSpaces.size() == 0) {
			taskAddedCondition.wait_for(lock, std::chrono::milliseconds(MAX_TIME_TO_WAIT_TO_RECHECK_TASK_LIST));
		} else {
			usedSpacesAccessMutex.lock();
			if (usedSpaces.size() > 0) {
				result = usedSpaces.front();
				usedSpaces.pop_front();
				hasResult = true;
			}
			usedSpacesAccessMutex.unlock();
		}
	}
	return result;
}

void ResultHolder::stopThread() {
	if (thread->joinable()) {
		isRunning = false;
		taskAddedCondition.notify_all();
		thread->join();
	}
}

int** ResultHolder::getResult() {
	stopThread();
	return result;
}