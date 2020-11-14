/*
 * SimpleAnomalyDetector.cpp
 *
 * Author: Adam Shapira; 3160044809
 */
#include "anomaly_detection_util.h"
#include "SimpleAnomalyDetector.h"
#include <algorithm>

static float const CORRELATION_THRESHOLD = 0.9;

SimpleAnomalyDetector::SimpleAnomalyDetector() = default;

SimpleAnomalyDetector::~SimpleAnomalyDetector() = default;


void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
	auto features = ts.getFeatureNames();
	auto numOfFeatures = features.size();
	auto const numOfSamples = ts.getNumOfSamples();
	// going through all the pairs of features
	for (auto i = 0; i < numOfFeatures; i++) {
		for (auto j = i + 1; j < numOfFeatures; j++) {
			auto const &feature2 = ts.getFeatureData(features[j]);
			auto const &feature1 = ts.getFeatureData(features[i]);
			auto correlation = pearson(feature1, feature2);
			//creating the points array and the line regression
			if (fabs(correlation) >= CORRELATION_THRESHOLD) {
				std::vector<unique_ptr<Point>> points;
				for (auto k = 0; k < numOfSamples; k++) {
					points.push_back(unique_ptr<Point>(new Point(feature1.at(k), feature2.at(k))));
				}
				auto const &lineReg = linear_reg(points);
				
				//calculating the max deviation for all the points
				auto it = std::max_element(points.begin(),
				                           points.end(),
				                           [&lineReg](const unique_ptr<Point> &p1, const unique_ptr<Point> &p2) {
					                           return dev(*p1, lineReg) < dev(*p2, lineReg);
				                           }
				);
				float threshold = dev(**it, lineReg) * 1.15f;
				cf.push_back(
						correlatedFeatures{
								(features[i]),
								(features[j]),
								correlation,
								lineReg,
								threshold
						}
				);
			}
		}
	}
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
	vector<AnomalyReport> anomalyReport;
	auto const numOfSamples = ts.getNumOfSamples();
	auto const &timeSteps = ts.getTimeSteps();
	for (auto const &correlatedFeatures:cf) {
		for (auto sample = 0; sample < numOfSamples; sample++) {
			auto const &feature1 = ts.getFeatureData(correlatedFeatures.feature1);
			auto const &feature2 = ts.getFeatureData(correlatedFeatures.feature2);
			auto const &p = Point(feature1.at(sample), feature2.at(sample));
			auto d = dev(p, correlatedFeatures.lin_reg);
			
			if (d > correlatedFeatures.threshold) {
				anomalyReport.emplace_back(
						correlatedFeatures.feature1 + "-" + correlatedFeatures.feature2,
						timeSteps.at(sample)
				);
			}
		}
	}
	return anomalyReport;
}