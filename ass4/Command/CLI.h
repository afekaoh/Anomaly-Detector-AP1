/*
* Author: Adam Shapira; 3160044809
 */

#ifndef CLI_H_
#define CLI_H_

#include <string>
#include "commands.h"
#include "../Anomaly-detection/HybridAnomalyDetector.h"

using namespace std;

class CLI {
	DefaultIO* dio;
	HybridAnomalyDetector anomalyDetector;
public:
	explicit CLI(DefaultIO* dio);
	
	// initiate the CLI
	void start();
	
	virtual ~CLI();
};

#endif /* CLI_H_ */
