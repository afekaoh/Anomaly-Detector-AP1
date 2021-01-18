

#ifndef CLI_H_
#define CLI_H_

#include <string>
#include "commands.h"

using namespace std;

class CLI {
	DefaultIO* _dio;
	HybridAnomalyDetector _anomalyDetector;
public:
	explicit CLI(DefaultIO* dio);
	
	// initiate the CLI
	void start();
	
	virtual ~CLI();
};

#endif /* CLI_H_ */
