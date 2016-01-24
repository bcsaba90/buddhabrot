#ifndef DATABASE_H
#define DATABASE_H
#include <string>
#include <fstream>
#include <set>
#include <random>
#include "complex.h"

class Database {
	std::fstream stream;
	std::set<Complex> points;
	std::set<Complex>::iterator iterator;
	bool hasNextEntryInFile();
	Complex readNextEntryFromFile();
	void moveIterator(int number);
	public:
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
