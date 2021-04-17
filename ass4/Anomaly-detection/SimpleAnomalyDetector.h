

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "AnomalyDetector.h"
#include "anomaly_detection_util.h"
#include "minCircle.h"
#include <algorithm>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

struct correlatedFeatures {
	
	std::string feature1, feature2; // names of the correlated features
	float corrlation;
	Line lin_reg;
	float threshold;
	Circle min_circle;
	
	correlatedFeatures(std::string feature1, std::string feature2,
	                   float corrlation, Line const &linReg, float threshold,
	                   Circle const &minCircle)
			: feature1(std::move(feature1)), feature2(std::move(feature2)), corrlation(corrlation),
			  lin_reg(linReg), threshold(threshold), min_circle(minCircle) {}
	
	correlatedFeatures(std::string feature1, std::string feature2,
	                   float corrlation) : feature1(std::move(feature1)),
	                                       feature2(std::move(feature2)),
	                                       corrlation(corrlation), threshold(0) {
		
	}
	
	
	friend ostream &operator<<(ostream &os, correlatedFeatures const &features) {
		os << "feature1: " << features.feature1 << " feature2: " << features.feature2
		   << " corrlation: " << features.corrlation << " threshold: " << features.threshold;
		return os;
	}
};


class AnomalyDetector : public TimeSeriesAnomalyDetector {
	float correlationThreshold = 0.9;
public:
	void setCorrelationThreshold(float threshold);
	
	float getCorrelationThreshold() const;
};

class SimpleAnomalyDetector : public AnomalyDetector {
	vector<correlatedFeatures> cf;
protected:
	correlatedFeatures getMaxCorr(TimeSeries const &ts,
	                              vector<string>::iterator const &endIterator,
	                              vector<string>::iterator &feature1Iterator);
	
	void createCorrelatedFeatures(TimeSeries const &ts, correlatedFeatures &cf1);
	
	virtual void fillCF(correlatedFeatures &cf1, vector<unique_ptr<Point>> &points) const;
	
	virtual bool toPush(correlatedFeatures const &cf1) const;
	
	virtual float
	getDistance(correlatedFeatures const &correlatedFeatures, Point const &point) const;

public:
	
	SimpleAnomalyDetector();
	
	virtual ~SimpleAnomalyDetector();
	
	// learning and saving the normal correlations of the Time Series
	virtual void learnNormal(const TimeSeries &ts);
	
	// creating a __report for all deviated data
	virtual std::vector<AnomalyReport> detect(const TimeSeries &ts);
	
	vector<correlatedFeatures> getNormalModel() { return cf; }
};

#endif /* SIMPLEANOMALYDETECTOR_H_ */
