

#ifndef ANOMALYDETECTOR_H_
#define ANOMALYDETECTOR_H_

#include <string>
#include <utility>
#include <vector>
#include "timeseries.h"
#include "math.h"

using namespace std;


class AnomalyReport {
public:
	const string description;
	const long timeStep;
	
	AnomalyReport(string description, long timeStep) : description(std::move(description)),
	                                                   timeStep(timeStep) {}
};

class TimeSeriesAnomalyDetector {
public:
	virtual void learnNormal(const TimeSeries &ts) = 0;
	
	virtual vector<AnomalyReport> detect(const TimeSeries &ts) = 0;
	
	virtual ~TimeSeriesAnomalyDetector() = default;
};

#endif /* ANOMALYDETECTOR_H_ */
