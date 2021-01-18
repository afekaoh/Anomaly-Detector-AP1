#include "CLI.h"

CLI::CLI(DefaultIO* dio) {
	this->_dio = dio;
	this->_anomalyDetector = HybridAnomalyDetector();
}

void CLI::start() {
	Command* cliCommand = new CliCommand(_dio, this->_anomalyDetector);
	cliCommand->execute();
	delete cliCommand;
}

CLI::~CLI() = default;

