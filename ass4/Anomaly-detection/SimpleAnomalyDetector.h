

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

struct correlatedFeatures {
	
	std::string feature1, feature2; // names of the correlated features
	float corrlation;
	Line lin_reg;
	float threshold;
	Circle min_circle;
	
	correlatedFeatures(std::string const &feature1, std::string const &feature2,
	                   float corrlation, Line const &linReg, float threshold,
	                   Circle const &minCircle)
			: feature1(feature1), feature2(feature2), corrlation(corrlation),
			  lin_reg(linReg), threshold(threshold), min_circle(minCircle) {}
	
	correlatedFeatures(std::string const &feature1, std::string const &feature2,
	                   float corrlation) {
		this->feature1 = feature1;
		this->feature2 = feature2;
		this->corrlation = corrlation;
		this->threshold = 0;
	}
	
	
	friend ostream &operator<<(ostream &os, correlatedFeatures const &features) {
		os << "feature1: " << features.feature1 << " feature2: " << features.feature2
		   << " corrlation: " << features.corrlation << " threshold: " << features.threshold;
		return os;
	}
};

class SimpleAnomalyDetector : public TimeSeriesAnomalyDetector {
	vector<correlatedFeatures> cf;
	float correlationThreshold;

protected:
	
	correlatedFeatures getMaxCorr(TimeSeries const &ts,
	                              vector<string>::iterator const &endIterator,
	                              vector<string>::iterator &feature1Iterator);
	
	virtual bool
	isDev(correlatedFeatures const &correlatedFeatures, Point const &point)
	const;
	
	void createCorrealatedFeatures(TimeSeries const &ts, correlatedFeatures &cf1);
	
	virtual void fillCF(correlatedFeatures &cf1, vector<unique_ptr<Point>> &points) const;
	
	virtual bool toPush(correlatedFeatures const &cf1) const;

public:
	
	SimpleAnomalyDetector();
	
	virtual ~SimpleAnomalyDetector();
	
	// learning and saving the normal correlations of the Time Series
	virtual void learnNormal(const TimeSeries &ts);
	
	// creating a __report for all deviated data
	virtual std::vector<AnomalyReport> detect(const TimeSeries &ts);
	
	vector<correlatedFeatures> getNormalModel() { return cf; }
	
	void setCorrelationThreshold(float threshold);
	
	float getCorrelationThreshold() const;
};

#endif /* SIMPLEANOMALYDETECTOR_H_ */
