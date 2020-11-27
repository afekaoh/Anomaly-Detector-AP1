/*
 * SimpleAnomalyDetector.cpp
 *
 * Author: Adam Shapira; 3160044809
 */
#include "anomaly_detection_util.h"
#include "SimpleAnomalyDetector.h"
#include <algorithm>
#include <iostream>

static float const CORRELATION_THRESHOLD = 0.9;

SimpleAnomalyDetector::SimpleAnomalyDetector() = default;

SimpleAnomalyDetector::~SimpleAnomalyDetector() = default;

void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
	auto features = ts.getFeatureNames();
	auto maxIt = features.begin();
	const auto &endIterator = features.end();
	// going through all the pairs of features
	auto featureIt = features.begin();
	for (; featureIt != endIterator - 1; featureIt++) {
		float maxCor = 0;
		auto featureIt2 = featureIt + 1;
		maxIt = featureIt2;
		auto const &feature1 = ts.getFeatureData(*featureIt);
		for (; featureIt2 != endIterator; featureIt2++) {
			auto const &feature2 = ts.getFeatureData(*featureIt2);
			auto correlation = pearson(feature1, feature2);
			//creating the points array and the line regression
			if (fabs(correlation) >= CORRELATION_THRESHOLD) {
				if (fabs(maxCor) < fabs(correlation)) {
					maxCor = correlation;
					maxIt = featureIt2;
					std::cout << maxCor << std::endl;
				}
			}
		}
		if (maxCor != 0) {
			auto const &feature2 = ts.getFeatureData(*maxIt);
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
			cf.push_back(
					correlatedFeatures{
							*featureIt,
							*maxIt,
							maxCor,
							lineReg,
							threshold
					}
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