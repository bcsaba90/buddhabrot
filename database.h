#ifndef DATABASE_H
#define DATABASE_H
#include <string>
#include <fstream>
#include <unordered_set>
#include <random>
#include "complex.h"

namespace std {
	template <>
	struct hash<Complex>
	{
		std::size_t operator()(const Complex& complex) const
		{
		  return (std::hash<double>()(complex.real) ^ (std::hash<double>()(complex.imaginary) << 1));
		}
	};
}

class Database {
	std::fstream stream;
	std::unordered_set<Complex> points;
	std::unordered_set<Complex>::iterator iterator;
	std::unordered_set<Complex> newPoints;
	bool hasNextEntryInFile();
	Complex readNextEntryFromFile();
	void moveIterator(int number);
	void writeToFile();
	public:
		Database();
		Database(const std::string& fileName);
		~Database();
		
		void reset(int offset);
		bool hasMoreElement();
		Complex getNextEntry(int offset);
		void writeEntry(const Complex& complex);
		void lastValueSuccess(long long iterationCount);
		int getSize();
};

#endif
