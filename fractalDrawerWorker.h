#ifndef FRACTAL_DRAWER_WORKER
#define FRACTAL_DRAWER_WORKER
#include "resultHolder.h"
#include "workProvider.h"
#include "progressIndicator.h"
#include "valueProvider.h"
#include <thread>

class FractalDrawerWorker {
	std::thread* thread;
	
	FractalParams& params;
	ResultHolder& resultHolder;
	WorkProvider& workProvider;
	ProgressIndicator& progressIndicator;
	ValueProvider& valueProvider;
	unsigned int threadIndex;
	
	bool isRunning = false;
	
	void increaseCountAndLogProgress(long long& countSinceLastLog);
	void increaseValue(long long iterationCount, Complex* route);
	void run();
	
	public:
		FractalDrawerWorker(ResultHolder& resultHolder, WorkProvider& workProvider, ProgressIndicator& progressIndicator, ValueProvider& valueProvider, FractalParams& params, unsigned int threadIndex);
		~FractalDrawerWorker();
		
		void start();
		void waitUnitFinished();
};

#endif