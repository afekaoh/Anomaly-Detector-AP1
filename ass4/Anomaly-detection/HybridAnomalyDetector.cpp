
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() = default;

HybridAnomalyDetector::~HybridAnomalyDetector() = default;


bool HybridAnomalyDetector::toPush(const correlatedFeatures &cf1) const {
	return abs(cf1.corrlation) > 0.5;
}

bool HybridAnomalyDetector::isDev(const correlatedFeatures &correlatedFeatures,
                                  const Point &point) const {
	if (abs(correlatedFeatures.corrlation) > getCorrelationThreshold()) {
		return SimpleAnomalyDetector::isDev(correlatedFeatures, point);
	} else
		return distance(point, correlatedFeatures.min_circle.center) > correlatedFeatures.threshold;
}


void
HybridAnomalyDetector::fillCF(correlatedFeatures &cf1, vector<unique_ptr<Point>> &points) const {
	if (abs(cf1.corrlation) > getCorrelationThreshold()) {
		SimpleAnomalyDetector::fillCF(cf1, points);
		return;
	}
	if (abs(cf1.corrlation) > 0.5) {
		cf1.min_circle = findMinCircle(points.data(), points.size());
		cf1.threshold = cf1.min_circle.radius * 1.1f;
	}
}
