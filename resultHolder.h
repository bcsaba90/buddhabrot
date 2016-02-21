#ifndef RESULT_HOLDER_H
#define RESULT_HOLDER_H
#include <list>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "complex.h"
#include "fractalParams.h"

struct ResultTempElement {
	Complex* result;
	unsigned long long size;
	
	ResultTempElement() : result(nullptr), size(0L) {}
	
	ResultTempElement(Complex* result, unsigned long long size) {
		this->result = result;
		this->size = size;
	}
};

class ResultHolder {
	std::mutex freeSpacesAccessMutex;
	std::mutex usedSpacesAccessMutex;
	std::thread* thread;
	std::condition_variable taskAddedCondition;
	std::mutex taskAddedMutex;
	
	bool isRunning;
	long long iterationLimit;
	FractalParams& params;
	int** result;
	
	std::list<Complex*> ownedSpaces;
	std::list<Complex*> freeSpaces;
	std::list<ResultTempElement> usedSpaces;
	
	void stopThread();

	ResultTempElement getResultToProcess();
	public:
		ResultHolder(FractalParams& fractalParams, unsigned int colorStepIndex);
		~ResultHolder();
		
		void initialize();
		void run();
		
		void addToResult(Complex* listToAdd, long long size);
		Complex* getTempResult();
		
		int** getResult();
		
		ResultHolder& operator=(const ResultHolder&) = delete;
};

#endif