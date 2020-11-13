/*
 * SimpleAnomalyDetector.cpp
 *
 * Author: Adam Shapira; 3160044809
 */
#include "SimpleAnomalyDetector.h"
#include "anomaly_detection_util.h"
#include <algorithm>
#include <iostream>

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
			auto const &feature1 = ts.getFeatureData(features[i]);
			auto const &feature2 = ts.getFeatureData(features[j]);
			auto correlation = pearson(feature1.data(), feature2.data(), numOfSamples);
			//creating the points array and the line regression
			if (fabs(correlation) >= CORRELATION_THRESHOLD) {
				Point* points[numOfSamples];
				for (auto k = 0; k < numOfSamples; k++) {
					points[k] = new Point(feature1.at(k), feature2.at(k));
				}
				Line const &lineReg = linear_reg(points, numOfSamples);
				
				//calculating the max deviation for all the points
				auto threshold = 0.0f;
				float tempDev;
				for_each(points, points + numOfSamples, [&tempDev, &threshold, &lineReg](Point*&p) {
					if (threshold < (tempDev = dev(*p, lineReg)))
						threshold = tempDev;
					//we dont need the point anymore
					delete p;
				});
				threshold *= 1.1;
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
	for (auto sample = 0; sample < numOfSamples; sample++) {
		for (const auto &cF:cf) {
			auto const feature1 = ts.getFeatureData(cF.feature1);
			auto const feature2 = ts.getFeatureData(cF.feature2);
			auto const p = Point(feature1.at(sample), feature2.at(sample));
			auto d = dev(p, cF.lin_reg);
			
			if (d > cF.threshold) {
				anomalyReport.emplace_back(cF.feature1 + "-" + cF.feature2, sample + 1);
			}
		}
	}
	return anomalyReport;
}
