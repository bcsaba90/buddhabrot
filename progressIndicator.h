#ifndef PROGRESS_INDICATOR
#define PROGRESS_INDICATOR
#include <mutex>
#include <thread>
#include <condition_variable>
#include <string>
#include <atomic>
#include "fractalParams.h"

class ProgressIndicator {
	std::mutex writeMutex;
	std::condition_variable writeCondition;
	std::mutex textsToWriteAccessMutex;
	std::atomic<unsigned long long> count;
	std::thread* thread;
	double lastPercent = 0.0;
	
	int currentColorStep = 0;
	FractalParams& params;
	bool isRunning = false;
	void stopThread();
	public:
		ProgressIndicator(FractalParams& params);
		~ProgressIndicator();
		
		void initialize();
		void run();
		void addNumberOfSamples(long long numberOfSamples);
		
		void increaseColorStep();
		
		ProgressIndicator& operator=(const ProgressIndicator&) = delete;
};

#endif