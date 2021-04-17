

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector : public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	
	virtual ~HybridAnomalyDetector();
	
	bool toPush(correlatedFeatures const &cf1) const override;


protected:
	void fillCF(correlatedFeatures &cf1, vector<unique_ptr<Point>> &points) const override;
	
	float
	getDistance(correlatedFeatures const &correlatedFeatures, Point const &point) const override;
	
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
