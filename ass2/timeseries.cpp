
#include "timeseries.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

/*
 * timeseries.cpp
 *
 * Author: Adam Shapira; 3160044809
 */
TimeSeries::TimeSeries(char const* CSVfileName, int timeStepIndex) {
	this->timeStepsIndex = timeStepIndex;
	createTable(CSVfileName);
	
}

TimeSeries::TimeSeries(const char* csvFileName) {
	this->timeStepsIndex = 0;
	createTable(csvFileName);
}

// saving all the csv data in a map for future use
void TimeSeries::createTable(char const* csvFileName) {
	// opening the csv
	ifstream data(csvFileName);
	string row;
	string word;
	char delim = ',';
	
	if (!data.is_open()) {
		throw runtime_error("error opening file");
	}
	
	// creating the features table and initialized the main table
	if (data.good()) {
		getline(data, row);
		stringstream rowStream(row);
		while (getline(rowStream, word, delim)) {
			features.push_back(word);
			dataTable.emplace(word, vector<float>());
		}
	}
	int colIndex = 0;
	auto length = features.size();
	vector<insert_iterator<vector<float, allocator<float>>>> its;
	while (getline(data, row)) {
		stringstream rowStream(row);
		std::for_each(features.begin(), features.end(), [&](std::string &s) {
			its.push_back(inserter(dataTable.at(s), dataTable.at(s).begin()));
		});
		while (getline(rowStream, word, delim)) {
			// converting column based table (each feature in a column) to a row based table (each feature in a row)
			its[(colIndex++) % length] = stof(word);
		}
	}
	data.close();
}

//returning the num of samples in the time series.
int TimeSeries::getNumOfSamples() const {
	return dataTable.at(features.at(0)).size();
}

// returning all the data of a given feature
vector<float> TimeSeries::getFeatureData(const std::string &feature) const {
	return dataTable.at(feature);
}

// returning all the features name only
vector<string> TimeSeries::getFeatureNames() const {
	return features;
}

const std::vector<float> TimeSeries::getTimeSteps() const {
	return dataTable.at(features[0]);
}
