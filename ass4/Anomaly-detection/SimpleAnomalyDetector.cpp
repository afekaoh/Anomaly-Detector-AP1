/*
 * SimpleAnomalyDetector.cpp
 *
 * Author: Adam Shapira; 3160044809
 */
#include "anomaly_detection_util.h"
#include "SimpleAnomalyDetector.h"
#include <algorithm>
#include <memory>

SimpleAnomalyDetector::SimpleAnomalyDetector() {
	this->cf = vector<correlatedFeatures>();
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() = default;


void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
	auto features = ts.getFeatureNames();
	// gets value just that i can use auto the value will be override
	const auto &endIterator = features.end();
	// going through all the pairs of features
	auto feature1Iterator = features.begin();
	
	
	/* runs from the first feature to the one before last no need to check the
	 last one as i check them in pairs */
	for (; feature1Iterator != endIterator - 1; feature1Iterator++) {
		correlatedFeatures cf1 = getMaxCorr(ts, endIterator, feature1Iterator);
		if (toPush(cf1)) {
			createCorrelatedFeatures(ts, cf1);
		}
	}
}

bool SimpleAnomalyDetector::toPush(correlatedFeatures const &cf1) const {
	return abs(cf1.corrlation) > this->getCorrelationThreshold();
}

void
SimpleAnomalyDetector::createCorrelatedFeatures(TimeSeries const &ts, correlatedFeatures &cf1) {
	// creating the data points
	auto const &feature1 = ts.getFeatureData(cf1.feature1);
	auto const &feature2 = ts.getFeatureData(cf1.feature2);
	vector<unique_ptr<Point>> points;
	detect_util::for_each_2(feature1.begin(), feature2.begin(), feature1.end(),
	                        feature2.end(),
	                        [&points](const float &f1, const float &f2) {
		                        points.push_back(unique_ptr<Point>(new Point(f1, f2)));
	                        });
	//
	fillCF(cf1, points);
	
	cf.push_back(cf1);
}

void
SimpleAnomalyDetector::fillCF(correlatedFeatures &cf1, vector<unique_ptr<Point>> &points) const {
	cf1.lin_reg = linear_reg(points);
	
	// calculating the max deviation for all the points
	auto const &line_reg = cf1.lin_reg;
	auto it = max_element(
			points.begin(), points.end(),
			[&line_reg](const unique_ptr<Point> &p1, const unique_ptr<Point> &p2) {
				return dev(*p1, line_reg) < dev(*p2, line_reg);
			});
	cf1.threshold = dev(**it, line_reg) * 1.15f;
}

correlatedFeatures
SimpleAnomalyDetector::getMaxCorr(TimeSeries const &ts, vector<string>::iterator const &endIterator,
                                  vector<string>::iterator &feature1Iterator) {
	float maxCor = 0;
	auto const &feature1 = ts.getFeatureData(*feature1Iterator);
	auto feature2Iterator = feature1Iterator + 1;
	auto feature2MaxIterator = feature2Iterator;
	for (; feature2Iterator != endIterator; feature2Iterator++) {
		auto const &feature2 = ts.getFeatureData(*feature2Iterator);
		auto correlation = pearson(feature1, feature2);
		// creating the points array and the line regression
		if (fabs(maxCor) < fabs(correlation)) {
			maxCor = correlation;
			feature2MaxIterator = feature2Iterator;
		}
	}
	return correlatedFeatures(*feature1Iterator, *feature2MaxIterator,
	                          maxCor);
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
	vector<AnomalyReport> anomalyReport;
	for (auto const &correlatedFeatures : cf) {
		auto timeSteps = 1;
		auto const &feature1 = ts.getFeatureData(correlatedFeatures.feature1);
		auto const &feature2 = ts.getFeatureData(correlatedFeatures.feature2);
		detect_util::for_each_2(
				feature1.begin(), feature2.begin(), feature1.end(), feature2.end(),
				[this, &anomalyReport, &timeSteps, &correlatedFeatures](const float &f1,
				                                                        const float &f2) {
					
					Point const &point = Point(f1, f2);
					if (getDistance(correlatedFeatures, point) > correlatedFeatures.threshold) {
						anomalyReport.emplace_back(correlatedFeatures.feature1 + "-" +
						                           correlatedFeatures.feature2,
						                           timeSteps);
					}
					timeSteps++;
				});
	}
	return anomalyReport;
}

float SimpleAnomalyDetector::getDistance(correlatedFeatures const &correlatedFeatures,
                                         Point const &point) const {
	return dev(point, correlatedFeatures.lin_reg);
}

void AnomalyDetector::setCorrelationThreshold(float threshold) {
	AnomalyDetector::correlationThreshold = threshold;
}

float AnomalyDetector::getCorrelationThreshold() const {
	return correlationThreshold;
}