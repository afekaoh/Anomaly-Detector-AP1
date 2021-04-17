#include "CLI.h"

CLI::CLI(DefaultIO* dio) : dio(dio), anomalyDetector(HybridAnomalyDetector()) {}

void CLI::start() {
	Command* cliCommand = new CliCommand(dio, anomalyDetector);
	cliCommand->execute();
	delete cliCommand;
}

CLI::~CLI() = default;

