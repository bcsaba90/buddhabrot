#ifndef FRACTAL_PARSET_H
#define FRACTAL_PARSER_H
#include "fractalParams.h"
#include <string>
#include <vector>
#include <map>

class FractalParser {
	std::vector<std::string> readAllLinesFromFile(const std::string& fileName);
	std::vector<std::string>& removeCommentsAndWhitespaces(std::vector<std::string>& lines);
	std::map<std::string, std::string> parseProperties(const std::vector<std::string>& lines);
	long long parsePropertyToInt(std::map<std::string, std::string>& properties, std::string propertyName, long long defaultValue);
	double parsePropertyToDouble(std::map<std::string, std::string>& properties, std::string propertyName, double defaultValue);
	bool parsePropertyToBool(std::map<std::string, std::string>& properties, std::string propertyName, bool defaultValue);
	std::string parsePropertyToString(std::map<std::string, std::string>& properties, std::string propertyName, const std::string& defaultValue);
	Vector parsePropertyToVector(std::map<std::string, std::string>& properties, std::string propertyName, const Vector& defaultValue);
	std::vector<ColorStep> parseColors(const std::vector<std::string>& lines);
	public:
		FractalParams readFractal(std::string fileName);
};
#endif
