#include "workProvider.h"
#include <cmath>

WorkProvider::WorkProvider(FractalParams& params, unsigned int colorStepIndex, long long workUnitSize, double power, bool saveTemporaryResult) {
	numberOfWorkUnits = (int)ceil((double)params.colorStep[colorStepIndex].iterationCount / workUnitSize);
	std::cout << "WorkUnits: " << numberOfWorkUnits << std::endl;
	workUnits = new WorkUnit*[numberOfWorkUnits];
	ColorStep& colorStep = params.colorStep[colorStepIndex];
	for (long long i = 0, j = 0; i < colorStep.iterationCount && j < numberOfWorkUnits; i += workUnitSize, ++j) {
		workUnits[j] = new WorkUnit();
		workUnits[j]->index = colorStepIndex;
		workUnits[j]->iterationLimit = params.colorStep[colorStepIndex].iterationLimit;
		int iterationCount = params.colorStep[colorStepIndex].iterationCount;
		workUnits[j]->iterationCount = (i + workUnitSize < iterationCount ? workUnitSize : iterationCount - i);
		workUnits[j]->minIterationCount = params.colorStep[colorStepIndex].minIterationCount;
		workUnits[j]->power = power;
		workUnits[j]->saveTemporaryResult = saveTemporaryResult;
	}
	currentWorkUnitIndex = 0;
}

WorkProvider::~WorkProvider() {
	for (int i = 0; i < numberOfWorkUnits; ++i) {
		delete workUnits[i];
	}
	delete[] workUnits;
}

WorkUnit* WorkProvider::getWorkUnit() {
	WorkUnit* result = nullptr;
	workUnitAccessMutex.lock();
	if (currentWorkUnitIndex < numberOfWorkUnits) {
		result = workUnits[currentWorkUnitIndex];
		++currentWorkUnitIndex;
	}
	workUnitAccessMutex.unlock();
	return result;
}
