/*
 * SimpleAnomalyDetector.cpp
 *
 * Author: Adam Shapira; 3160044809
 */
#include "anomaly_detection_util.h"
#include "SimpleAnomalyDetector.h"
#include <algorithm>
#include <iostream>

SimpleAnomalyDetector::SimpleAnomalyDetector() {
	this->cf = vector<correlatedFeatures>();
	this->correlationThreshold = 0.9;
}

void SimpleAnomalyDetector::setCorrelationThreshold(float correlationThreshold) {
	SimpleAnomalyDetector::correlationThreshold = correlationThreshold;
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() = default;

void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
	auto features = ts.getFeatureNames();
	// gets value just that i can use auto the value will be override
	auto feature2MaxIterator = features.begin();
	const auto &endIterator = features.end();
	// going through all the pairs of features
	auto feature1Iterator = features.begin();
	
	/* runs from the first feature to the one before last no need to check the last one as i check them
	 in pairs */
	for (; feature1Iterator != endIterator - 1; feature1Iterator++) {
		float maxCor = 0;
		auto feature2Iterator = feature1Iterator + 1;
		feature2MaxIterator = feature2Iterator;
		auto const &feature1 = ts.getFeatureData(*feature1Iterator);
		for (; feature2Iterator != endIterator; feature2Iterator++) {
			auto const &feature2 = ts.getFeatureData(*feature2Iterator);
			auto correlation = pearson(feature1, feature2);
			//creating the points array and the line regression
			if (fabs(correlation) >= this->correlationThreshold &&
			    fabs(maxCor) < fabs(correlation)) {
				maxCor = correlation;
				feature2MaxIterator = feature2Iterator;
			}
		}
		if (maxCor != 0) {
			auto const &feature2 = ts.getFeatureData(*feature2MaxIterator);
			std::vector<unique_ptr<Point>> points;
			detect_util::for_each_2(feature1.begin(),
			                        feature2.begin(),
			                        feature1.end(),
			                        feature2.end(),
			                        [&points](const float &f1, const float &f2) {
				                        points.push_back(unique_ptr<Point>(new Point(f1, f2)));
			                        }
			);
			
			auto const &lineReg = linear_reg(points);
			
			//calculating the max deviation for all the points
			auto it = std::max_element(points.begin(),
			                           points.end(),
			                           [&lineReg](const unique_ptr<Point> &p1,
			                                      const unique_ptr<Point> &p2) {
				                           return dev(*p1, lineReg) < dev(*p2, lineReg);
			                           }
			);
			float threshold = dev(**it, lineReg) * 1.15f;
			cf.emplace_back(
					*feature1Iterator,
					*feature2MaxIterator,
					maxCor,
					lineReg,
					threshold
			
			);
		}
	}
}


vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
	vector<AnomalyReport> anomalyReport;
	for (auto const &correlatedFeatures:cf) {
		auto timeSteps = ts.getTimeSteps().begin();
		auto const &feature1 = ts.getFeatureData(correlatedFeatures.feature1);
		auto const &feature2 = ts.getFeatureData(correlatedFeatures.feature2);
		detect_util::for_each_2(feature1.begin(),
		                        feature2.begin(),
		                        feature1.end(),
		                        feature2.end(),
		                        [&anomalyReport, &timeSteps, &correlatedFeatures](const float &f1,
		                                                                          const float &f2) {
			                        auto const &p = Point(f1, f2);
			                        auto d = dev(p, correlatedFeatures.lin_reg);
			                        if (d > correlatedFeatures.threshold) {
				                        anomalyReport.emplace_back(
						                        correlatedFeatures.feature1 + "-" +
						                        correlatedFeatures.feature2,
						                        *timeSteps
				                        );
			                        }
			                        timeSteps++;
		                        });
	}
	return anomalyReport;
}