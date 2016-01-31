#include "database.h"
#include "complex.h"
#include <iostream>

Database::Database(const std::string& fileName) {
	stream.open(fileName, std::ios::binary | std::ios::in | std::ios::out | std::fstream::out | std::ios::app);
	while (hasNextEntryInFile()) {
		Complex c = readNextEntryFromFile();
		points.insert(c);
	}
	std::cout << "Number of points loaded: " << points.size() << std::endl;
	iterator = points.begin();
}

Database::~Database() {
	writeToFile();
	stream.flush();
	stream.close();
}

void Database::reset(int offset) {
	iterator = points.begin();
	moveIterator(offset);
}

bool Database::hasNextEntryInFile() {
	int nextValue = stream.peek();
	return nextValue != EOF;
}

Complex Database::readNextEntryFromFile() {
	double real, imaginary;
	stream.read((char*)&real, sizeof(double));
	stream.read((char*)&imaginary, sizeof(double));
	return Complex(real, imaginary);
}

bool Database::hasMoreElement() {
	return iterator != points.end();
}

Complex Database::getNextEntry(int offset) {
	Complex result = *iterator;
	moveIterator(offset);
	return result;
}

void Database::writeEntry(const Complex& complex) {
	if (points.find(complex) == points.end() && newPoints.find(complex) == newPoints.end()) {
		newPoints.insert(complex);
	}
}

void Database::writeToFile() {
	for (auto it = newPoints.begin(); it != newPoints.end(); ++it) {
		const Complex& complex = *it;
		stream.seekp(0, std::ios_base::end);
		stream.clear();
		stream.write((char*)(&(complex.real)), sizeof(double));
		stream.write((char*)(&(complex.imaginary)), sizeof(double));
	}
}

void Database::moveIterator(int number) {
	for (int i = 0; i < number && iterator != points.end(); ++i) {
		++iterator;
	}
}
	
int Database::getSize() {
	return points.size();
}
