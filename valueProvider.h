#ifndef VALUE_PROVIDER
#define VALUE_PROVIDER
#include "database.h"
#include "complex.h"
#include "fractalParams.h"
#include <vector>
#include <mutex>
#include <unordered_set>

struct IntCoordinate {
	int x, y;
	
	IntCoordinate(int x_p, int y_p) : x(x_p), y(y_p){}
	
	bool operator==(const IntCoordinate& other) const {
		return x == other.x && y == other.y;
	}
};

namespace std {
	template <>
	struct hash<IntCoordinate>
	{
		std::size_t operator()(const IntCoordinate& coordinate) const
		{
		  return (std::hash<int>()(coordinate.x) ^ (std::hash<int>()(coordinate.y) << 1));
		}
	};
}

class ValueProvider {
	std::mutex writeInterestingQuadrantMutex;
	bool isWritingInterestingQuadrant = false;
	
	int iterationCountToConsiderInteresting = 100;
	Database* database;
	bool readAllDataFromDatabase = false;
	Complex previousValue;
	long long previousIterationCount;
	bool isLastValueMirroredAlready = false;
	bool lastValueSuccessful = false;
	bool readOnlyFile = false;
	bool noDb = false;
	FractalParams& params;
	unsigned int numberOfThreads;
  std::vector<IntCoordinate> interestingQuadrants;
	std::unordered_set<IntCoordinate> interestingQuadrantsSet;

  std::mt19937** generators;
  std::uniform_real_distribution<double>* randomDistribution;
  
	Complex chooseCoordinate(unsigned int threadIndex);
	Complex getNextValueInternal(unsigned int index, unsigned int threadIndex, bool isMandelbrot);
	bool isSurelyPartOfMandelbrot(const Complex& c);
	double randomDoubleInRange(double min, double max, unsigned int threadIndex);
	int randomIntInRange(int min, int max, unsigned int threadIndex);
	
	public:
		ValueProvider(Database* database, FractalParams& params, unsigned int numberOfThreads);
		~ValueProvider();
		
		Complex getNextValue(unsigned int index, unsigned int threadIndex, bool isMandelbrot);
		void lastValueSuccess(long long iterationCount);
		void deleteSavedValues();
		void reset(unsigned int index);
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
		void setNoDb(bool noDb) {
			this->noDb = noDb;
		}
};

#endif
