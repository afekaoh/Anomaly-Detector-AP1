

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <utility>
#include <vector>
#include <algorithm>
#include <string>
#include <ostream>


struct correlatedFeatures {
	
	std::string feature1, feature2;  // names of the correlated features
	float corrlation;
	Line lin_reg;
	float threshold;
	
	correlatedFeatures(const std::string &feature1, const std::string &feature2, float correlation,
	                   Line const &line_reg, float threshold) : feature1(feature1),
	                                                            feature2(feature2),
	                                                            corrlation(correlation),
	                                                            lin_reg(line_reg),
	                                                            threshold(threshold) {}
};


class SimpleAnomalyDetector : public TimeSeriesAnomalyDetector {
	vector<correlatedFeatures> cf;
	float correlationThreshold;
public:
	
	SimpleAnomalyDetector();
	
	virtual ~SimpleAnomalyDetector();
	
	// learning and saving the normal correlations of the Time Series
	virtual void learnNormal(const TimeSeries &ts);
	
	// creating a report for all deviated data
	virtual std::vector<AnomalyReport> detect(const TimeSeries &ts);
	
	vector<correlatedFeatures> getNormalModel() {
		return cf;
	}
	
	void setCorrelationThreshold(float correlationThreshold);
};


#endif /* SIMPLEANOMALYDETECTOR_H_ */
