#include "fractalParser.h"
#include "fractal.h"
#include "generalException.h"
#include "database.h"
#include "valueProvider.h"
#include "argumentParser.h"
#include <thread>

void showUsage(const std::vector<Argument>& arguments) {
	std::cout << "Fractal drawing program in C++\nUsage:\n./programName [arguments] -f fileToParse.frak\nFor example:\n./a.out -f \"./fractal_definitions/definition.frak\"\n\n\nOPTIONS:" << std::endl;
	for (Argument argument : arguments) {
		std::cout << "  -" << argument.shortName << ", --" << argument.longName << "\t\t" << argument.description << std::endl;
	}
}

std::vector<Argument> createArguments() {
	std::vector<Argument> arguments;
	arguments.push_back(Argument('h', "help", "Display this help", BOOLEAN_ARGUMENT));
	arguments.push_back(Argument('f', "filename", "Name of the file to render, mandatory argument", STRING_ARGUMENT));
	arguments.push_back(Argument('d', "databasefile", "Name of the db file to use, defaults to point.ff", STRING_ARGUMENT));
	arguments.push_back(Argument('o', "onlyfromdb", "Limit the amount of samples to the same as points in the database", BOOLEAN_ARGUMENT));
	arguments.push_back(Argument('b', "nodb", "Don't read or write to db", BOOLEAN_ARGUMENT));
	arguments.push_back(Argument('i', "interesting-iteration-count", "Iteration count to consider interesting, defaults to 100", INTEGER_ARGUMENT));
	arguments.push_back(Argument('t', "threads", "Number of threads, defaults to core count", INTEGER_ARGUMENT));
	return arguments;
}

bool areAllMandatoryArgumentsPresent(const ArgumentParser& argumentParser) {
	return argumentParser.isArgumentPresent('f');
}

int main(int argc, char** argv) {
	std::vector<Argument> arguments = createArguments();
	ArgumentParser argumentParser;
	try {
		argumentParser.parse(argc, argv, arguments);
	} catch(const GeneralException& exception) {
		std::cout << exception.what() << std::endl;
		showUsage(arguments);
		return -1;
	}
	if (argumentParser.isArgumentPresent('h')) {
		showUsage(arguments);
		return 0;
	}
	if (!areAllMandatoryArgumentsPresent(argumentParser)) {
		showUsage(arguments);
		return -1;
	}
	try {
		FractalParser parser;
		FractalParams params = parser.readFractal(argumentParser.getStringArgument('f'));
		Database* database;
		int hardwareConcurrency = std::thread::hardware_concurrency();
		if (hardwareConcurrency == 0) {
			hardwareConcurrency = 1;
		}
		int numberOfThreads = argumentParser.getIntArgumentOrReturnDefault('t', hardwareConcurrency);
		std::cout << "Number of threads: " << numberOfThreads << std::endl;
		if (argumentParser.isArgumentPresent('b')) {
			database = new Database();
		} else {
			database = new Database(argumentParser.getStringArgumentOrReturnDefault('d', "points.ff"));
		}
		ValueProvider* valueProvider = new ValueProvider(database, params, numberOfThreads);
		valueProvider->setIterationCountToConsiderInteresting(argumentParser.getIntArgumentOrReturnDefault('i', 100));
		if (argumentParser.isArgumentPresent('o')) {
			valueProvider->setReadOnlyFile(true);
		}
		if (argumentParser.isArgumentPresent('b')) {
			valueProvider->setNoDb(true);
		}
		for (double d = params.startPower; d <= params.endPower; d+= params.increment) {
			valueProvider->deleteSavedValues();
			Fractal fractal(valueProvider, params);
			fractal.draw(d, numberOfThreads);
		}
		delete database;
		delete valueProvider;
	} catch(const GeneralException& ex) {
		std::cout << "Exception occured: " << ex.what() << std::endl;
		return -1;
	}
	return 0;
}
