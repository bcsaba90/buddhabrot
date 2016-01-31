#include "valueProvider.h"
#include "numericHelper.h"
#include <algorithm>

const int MAX_NUMBER_OF_TRIES = 100;
const int ITERATION_COUNT_TO_CONSIDER_INTERESTING = 2000;
const int ITERATION_COUNT_NEEDED_FOR_SAVE = 1000;
const double QUADRANT_SIZE = 0.01;
const int MINIMUM_AMOUNT_OF_RANDOM_QUADRANT_BEFORE_USE = 10;

ValueProvider::ValueProvider(Database* database, FractalParams& params_p) : params(params_p) {
	this->database = database;
  generator = new std::mt19937(randomDevice());
  randomDistribution = new std::uniform_real_distribution<double>(0.0, 1.0);
}

ValueProvider::~ValueProvider() {
	std::cout << "interestingQuadrants: " << interestingQuadrants.size() << std::endl;
}

inline double ValueProvider::randomDoubleInRange(double min, double max) {
	return NumericHelper::map((*randomDistribution)(*generator), 0.0, 1.0, min, max);
}

bool ValueProvider::isSurelyPartOfMandelbrot(const Complex& c) {
	double q = (c.real - 1.0 / 4) * (c.real - 1.0 / 4) + c.imaginary * c.imaginary;
	return q * (q + (c.real - 1.0 / 4)) < 1.0 / 4 * c.imaginary * c.imaginary;
}


Complex ValueProvider::chooseCoordinate() {
	if ((double)rand() / RAND_MAX < params.randomChance && interestingQuadrants.size() > MINIMUM_AMOUNT_OF_RANDOM_QUADRANT_BEFORE_USE) {
		IntCoordinate interestingQuadrantCoordinate = interestingQuadrants[rand() % interestingQuadrants.size()];
		double real      = randomDoubleInRange(interestingQuadrantCoordinate.x * QUADRANT_SIZE, interestingQuadrantCoordinate.x * QUADRANT_SIZE + QUADRANT_SIZE);
		double imaginary = randomDoubleInRange(interestingQuadrantCoordinate.y * QUADRANT_SIZE, interestingQuadrantCoordinate.y * QUADRANT_SIZE + QUADRANT_SIZE);
		return Complex(real, imaginary);
	} else {
		double real      = randomDoubleInRange(params.minX, params.maxX);
		double imaginary = randomDoubleInRange(params.minY, params.maxY);
		return Complex(real, imaginary);
	}
}

Complex ValueProvider::getNextValue(int index) {
	previousValue = getNextValueInternal(params.colorStep.size());
	return previousValue;
}

Complex ValueProvider::getNextValueInternal(int index) {
	if (!readAllDataFromDatabase) {
		if (database->hasMoreElement()) {
			return database->getNextEntry(index);
		} else {
			readAllDataFromDatabase = true;
		}
	}
	if (lastValueSuccessful && !isLastValueMirroredAlready) {
		previousValue.imaginary *= -1.0;
		isLastValueMirroredAlready = true;
		return previousValue;
	}
	int j = 0;
	Complex choosen;
	while (j < MAX_NUMBER_OF_TRIES) {
		choosen = chooseCoordinate();
		if (!isSurelyPartOfMandelbrot(choosen)) {
			break;
		}
		++j;
	}
	if (isLastValueMirroredAlready) {
		isLastValueMirroredAlready = false;
	}
	return choosen;
}

void ValueProvider::lastValueSuccess(long long iterationCount) {
	lastValueSuccessful = true;
	previousIterationCount = iterationCount;
	if (iterationCount > ITERATION_COUNT_NEEDED_FOR_SAVE) {
		database->writeEntry(previousValue);
	}
	if (iterationCount > ITERATION_COUNT_TO_CONSIDER_INTERESTING) {
		IntCoordinate interestingQuadrant(floor(previousValue.getReal() / QUADRANT_SIZE), floor(previousValue.getImaginary() / QUADRANT_SIZE));
		if (std::find(interestingQuadrants.begin(), interestingQuadrants.end(), interestingQuadrant) == interestingQuadrants.end()) {
			interestingQuadrants.push_back(interestingQuadrant);
		}
	}
}

void ValueProvider::reset(int index) {
	database->reset(index);
}
