#ifndef VALUE_PROVIDER
#define VALUE_PROVIDER
#include "database.h"
#include "complex.h"
#include "fractalParams.h"
#include <vector>

struct IntCoordinate {
	int x, y;
	
	IntCoordinate(int x_p, int y_p) : x(x_p), y(y_p){}
	
	bool operator==(const IntCoordinate& other) {
		return x == other.x && y == other.y;
	}
};

class ValueProvider {
	int iterationCountToConsiderInteresting = 100;
	Database* database;
	bool readAllDataFromDatabase = false;
	Complex previousValue;
	long long previousIterationCount;
	bool isLastValueMirroredAlready = false;
	bool lastValueSuccessful = false;
	bool readOnlyFile = false;
	FractalParams& params;
  std::vector<IntCoordinate> interestingQuadrants;
  
  std::random_device randomDevice;
  std::mt19937* generator;
  std::uniform_real_distribution<double>* randomDistribution;
  
	Complex chooseCoordinate();
	Complex getNextValueInternal(int index);
	bool isSurelyPartOfMandelbrot(const Complex& c);
	double randomDoubleInRange(double min, double max);
	
	public:
		ValueProvider(Database* database, FractalParams& params);
		~ValueProvider();
		
		Complex getNextValue(int index);
		void lastValueSuccess(long long iterationCount);
		void reset(int index);
		void setReadOnlyFile(bool readOnlyFile) {
			this->readOnlyFile = readOnlyFile;
		}
		bool hasMoreValue() {
			if (readOnlyFile) {
				return !readAllDataFromDatabase;
			}
			return true;
		}
		void setIterationCountToConsiderInteresting(int iterationCountToConsiderInteresting) {
			this->iterationCountToConsiderInteresting = iterationCountToConsiderInteresting;
		}
};

#endif
