#include "valueProvider.h"
#include "numericHelper.h"
#include <algorithm>

const int MAX_NUMBER_OF_TRIES = 100;
const int MINIMUM_ITERATION_COUNT_TO_CONSIDER_INTERESTING = 40;
const int ITERATION_COUNT_NEEDED_FOR_SAVE = 1000;
const double QUADRANT_SIZE = 0.01;
const int MINIMUM_AMOUNT_OF_RANDOM_QUADRANT_BEFORE_USE = 10;

ValueProvider::ValueProvider(Database* database, FractalParams& params_p, unsigned int numberOfThreads) : params(params_p) {
	this->database = database;
	this->numberOfThreads = numberOfThreads;
  std::random_device randomDevice;
	generators = new std::mt19937*[numberOfThreads];
	for (unsigned int i = 0; i < numberOfThreads; ++i) {
		generators[i] = new std::mt19937(randomDevice());
	}
	
  randomDistribution = new std::uniform_real_distribution<double>(0.0, 1.0);
}

ValueProvider::~ValueProvider() {
	for (unsigned int i = 0; i < numberOfThreads; ++i) {
		delete generators[i];
	}
	delete[] generators;
	std::cout << "interestingQuadrants: " << interestingQuadrants.size() << std::endl;
}

inline double ValueProvider::randomDoubleInRange(double min, double max, unsigned int threadIndex) {
	return NumericHelper::map((*randomDistribution)(*(generators[threadIndex])), 0.0, 1.0, min, max);
}

inline int ValueProvider::randomIntInRange(int min, int max, unsigned int threadIndex) {
	return static_cast<int>(NumericHelper::map((*randomDistribution)(*(generators[threadIndex])), 0.0, 1.0, min, max));
}

bool ValueProvider::isSurelyPartOfMandelbrot(const Complex& c) {
	bool partOfMandelbrot = false;
	double q = (c.real - 1.0 / 4) * (c.real - 1.0 / 4) + c.imaginary * c.imaginary;
	partOfMandelbrot = (q * (q + (c.real - 1.0 / 4)) < 1.0 / 4 * c.imaginary * c.imaginary);
	partOfMandelbrot = partOfMandelbrot || ((c.real + 1) * (c.real + 1) + c.imaginary * c.imaginary < 1.0 / 16);
	return partOfMandelbrot;
}


Complex ValueProvider::chooseCoordinate(unsigned int threadIndex) {
	double chanceToGetQuadrant = (double)randomDoubleInRange(0.0, 1.0, threadIndex);
	int randomIndexInQuadrantVector = randomIntInRange(0, interestingQuadrants.size(), threadIndex);
	if (chanceToGetQuadrant < params.randomChance && interestingQuadrants.size() > MINIMUM_AMOUNT_OF_RANDOM_QUADRANT_BEFORE_USE && !isWritingInterestingQuadrant) {
		IntCoordinate interestingQuadrantCoordinate = interestingQuadrants[randomIndexInQuadrantVector];
		double real      = randomDoubleInRange(interestingQuadrantCoordinate.x * QUADRANT_SIZE, interestingQuadrantCoordinate.x * QUADRANT_SIZE + QUADRANT_SIZE, threadIndex);
		double imaginary = randomDoubleInRange(interestingQuadrantCoordinate.y * QUADRANT_SIZE, interestingQuadrantCoordinate.y * QUADRANT_SIZE + QUADRANT_SIZE, threadIndex);
		return Complex(real, imaginary);
	} else {
		double real      = randomDoubleInRange(params.minX, params.maxX, threadIndex);
		double imaginary = randomDoubleInRange(params.minY, params.maxY, threadIndex);
		return Complex(real, imaginary);
	}
}

Complex ValueProvider::getNextValue(unsigned int index, unsigned int threadIndex, bool isMandelbrot) {
	return getNextValueInternal(index, threadIndex, isMandelbrot);
}

Complex ValueProvider::getNextValueInternal(unsigned int index, unsigned int threadIndex, bool isMandelbrot) {
	if (!readAllDataFromDatabase && !noDb) {
		if (database->hasMoreElement()) {
			return database->getNextEntry(index);
		} else {
			readAllDataFromDatabase = true;
		}
	}
	int j = 0;
	Complex choosen;
	while (j < MAX_NUMBER_OF_TRIES) {
		choosen = chooseCoordinate(threadIndex);
		if (!isMandelbrot || !isSurelyPartOfMandelbrot(choosen)) {
			break;
		}
		++j;
	}
	return choosen;
}

void ValueProvider::lastValueSuccess(Complex& previousValue, long long iterationCount) {
	if (iterationCount > ITERATION_COUNT_NEEDED_FOR_SAVE && !noDb) {
		database->writeEntry(previousValue);
	}
	if (iterationCount > MINIMUM_ITERATION_COUNT_TO_CONSIDER_INTERESTING) {
		IntCoordinate interestingQuadrant(floor(previousValue.getReal() / QUADRANT_SIZE), floor(previousValue.getImaginary() / QUADRANT_SIZE));
		if (interestingQuadrantsSet.find(interestingQuadrant) == interestingQuadrantsSet.end()) {
			writeInterestingQuadrantMutex.lock();
			isWritingInterestingQuadrant = true;
			interestingQuadrants.push_back(interestingQuadrant);
			isWritingInterestingQuadrant = false;
			interestingQuadrantsSet.insert(interestingQuadrant);
			writeInterestingQuadrantMutex.unlock();
		}
	}
}

void ValueProvider::reset(unsigned int index) {
	database->reset(index);
}

void ValueProvider::deleteSavedValues() {
	interestingQuadrants.clear();
	interestingQuadrantsSet.clear();
}
