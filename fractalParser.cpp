#include "fractalParser.h"
#include <algorithm>
#include <fstream>
#include <iterator>
#include <cstring>
#include <sstream>
#include "generalException.h"

std::vector<std::string> FractalParser::readAllLinesFromFile(const std::string& fileName) {
	std::vector<std::string> result;
	
	std::ifstream in(fileName.c_str());
	if (!in.is_open()) {
		throw GeneralException("File cannot be opened");
	}
	
	std::string line;
	while (std::getline(in, line)) {
		  result.push_back(line);
	}
	
	return result;
}

std::vector<std::string>& FractalParser::removeCommentsAndWhitespaces(std::vector<std::string>& lines) {
	for (unsigned int i = 0; i < lines.size(); ++i) {
		std::string line = lines[i];
		line = line.substr(0, line.find("#"));
		
		const char* charsToRemove = " \t";
	  for (unsigned int j = 0; j < std::strlen(charsToRemove); ++j) {
      line.erase(std::remove(line.begin(), line.end(), charsToRemove[j]), line.end());
    }
		
		lines[i] = line;
	}
	return lines;
}

std::map<std::string, std::string> FractalParser::parseProperties(const std::vector<std::string>& lines) {
	std::map<std::string, std::string> result;
	
	for (unsigned int i = 0; i < lines.size(); ++i) {
		const std::string& line = lines[i];
		size_t equalPosition = line.find("=");
		if (equalPosition != std::string::npos) {
			result.insert(std::pair<std::string, std::string>(line.substr(0, equalPosition), line.substr(equalPosition + 1, std::string::npos)));
		}
	}
	
	return result;
}

long long parseLong(const std::string& stringToConvert) {
			long long result = 0;
			for (unsigned int i = 0; i < stringToConvert.size(); ++i) {
				result *= 10;
				result += (stringToConvert[i] - '0');
			}
			return result;
}

long long FractalParser::parsePropertyToInt(std::map<std::string, std::string>& properties, std::string propertyName, long long defaultValue) {
	if (properties.find(propertyName) != properties.end()) {
		try {
			std::string stringToConvert = properties[propertyName];
			return parseLong(stringToConvert);
		} catch (const std::exception& exception) {
			throw new GeneralException(std::string("Could not convert property ") + propertyName + " to int");
		}
	} else {
		std::cout << "Warning: " << propertyName << " not set, default " << defaultValue << " is used instead!" << std::endl;
		return defaultValue;
	}
}

double FractalParser::parsePropertyToDouble(std::map<std::string, std::string>& properties, std::string propertyName, double defaultValue) {
	if (properties.find(propertyName) != properties.end()) {
		try {
			return atof(properties[propertyName].c_str());
		} catch (const std::exception& exception) {
			throw new GeneralException(std::string("Could not convert property ") + propertyName + " to double");
		}
	} else {
		std::cout << "Warning: " << propertyName << " not set, default " << defaultValue << " is used instead!" << std::endl;
		return defaultValue;
	}
}


bool FractalParser::parsePropertyToBool(std::map<std::string, std::string>& properties, std::string propertyName, bool defaultValue) {
	if (properties.find(propertyName) != properties.end()) {
		std::string& property = properties[propertyName];
		return (property == "true" || property == "1");
	} else {
		std::cout << "Warning: " << propertyName << " not set, default " << defaultValue << " is used instead!" << std::endl;
		return defaultValue;
	}
}

Vector FractalParser::parsePropertyToVector(std::map<std::string, std::string>& properties, std::string propertyName, const Vector& defaultValue) {
	if (properties.find(propertyName) != properties.end()) {
		std::string& property = properties[propertyName];
		return Vector().parseFromString(property);		
	} else {
		std::cout << "Warning: " << propertyName << " not set, default " << defaultValue << " is used instead!" << std::endl;
		return defaultValue;
	}
}

std::string FractalParser::parsePropertyToString(std::map<std::string, std::string>& properties, std::string propertyName, const std::string& defaultValue) {
	if (properties.find(propertyName) != properties.end()) {
		return properties[propertyName];
	} else {
		std::cout << "Warning: " << propertyName << " not set, default " << defaultValue << " is used instead!" << std::endl;
		return defaultValue;
	}
}

std::vector<std::string> split(const std::string& str, char c)  {
	std::stringstream test(str);
	std::string segment;
	std::vector<std::string> seglist;

	while(std::getline(test, segment, c))
	{
		 seglist.push_back(segment);
	}
	return seglist;
}

std::vector<ColorStep> FractalParser::parseColors(const std::vector<std::string>& lines) {
	std::vector<ColorStep> result;
	unsigned int startLine = 0;
	for (startLine = 0; startLine < lines.size(); ++startLine) {
		if (lines[startLine].find("colorStep") == 0) {
			break;
		}
	}
	unsigned int i = startLine + 1;
	// parses (x,y,z) d d d 
	while (i < lines.size() && lines[i].find("}") == std::string::npos) {
		std::vector<std::string> parts = split(lines[i], '|');
		std::cout << lines[i] << std::endl;
		if (parts.size() != 5) {
			std::cout << "Skipping colorDefinition: not the right amount of parameters" << std::endl;
			continue;
		}
		ColorStep colorStep;
		colorStep.color.parseFromString(parts[0]);
		colorStep.iterationLimit = parseLong(parts[1]);
		colorStep.colorMultiplier = atof(parts[2].c_str());
		colorStep.iterationCount = parseLong(parts[3]);
		colorStep.minIterationCount = parseLong(parts[4]);
		result.push_back(colorStep);
		++i;
	}
	if (i == lines.size()) {
		std::cout << "Warning: missing '}'" << std::endl;
	}
	return result;
}

FractalParams FractalParser::readFractal(std::string fileName) {
	FractalParams result;
	std::vector<std::string> lines = readAllLinesFromFile(fileName);
	removeCommentsAndWhitespaces(lines);
	std::map<std::string, std::string> properties = parseProperties(lines);
	result.width = parsePropertyToInt(properties, "width", 1920);
	result.height = parsePropertyToInt(properties, "height", 1200);
	result.randomChance = parsePropertyToDouble(properties, "random_chance", 0.4);
	result.minX = parsePropertyToDouble(properties, "min_x", -2);
	result.maxX = parsePropertyToDouble(properties, "max_x", 2);
	result.minY = parsePropertyToDouble(properties, "min_y", -1.8);
	result.maxY = parsePropertyToDouble(properties, "max_y", 1.8);
	result.fileName = parsePropertyToString(properties, "filename", "buddhabrot");
	std::vector<ColorStep> colors = parseColors(lines);
	result.colorStep = colors;
	return result;
}
