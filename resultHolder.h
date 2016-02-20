#ifndef RESULT_HOLDER_H
#define RESULT_HOLDER_H
#include <list>
#include "complex.h"

class ResultHolder {
	long long maxIterationSize;
	int** result;
	std::list<Complex*> freeSpaces;
	std::list<Complex*> usedSpaces;
	
	public:
		ResultHolder(int width, int height, long long maxIterationSize);
		~ResultHolder();
		
		void addToResult(Complex* listToAdd, long long size);
		Complex* getTempResult();
};

#endif