/*
 * timeseries.cpp
*
* Author: Adam Shapira; 3160044809
 */


#include "timeseries.h"
#include <fstream>
#include <sstream>
#include <algorithm>


TimeSeries::TimeSeries(char const* CsvFileName, int timeStepIndex) {
	this->timeStepsIndex = timeStepIndex;
	createTable(CsvFileName);
}


// saving all the csv data in a map for future use
void TimeSeries::createTable(char const* csvFileName) {
	// opening the csv
	std::ifstream data(csvFileName);
	std::string row;
	std::string word;
	char delim = ',';
	
	if (!data.is_open()) {
		throw std::runtime_error("error opening file");
	}
	
	// creating the features table and initialized the main table
	if (data.good()) {
		getline(data, row);
		std::stringstream rowStream(row);
		while (getline(rowStream, word, delim)) {
			features.push_back(word);
			dataTable.emplace(word, std::vector<float>());
		}
		int colIndex = 0;
		auto length = features.size();
		std::vector<std::insert_iterator<std::vector<float, std::allocator<float>>>> its;
		while (getline(data, row)) {
			rowStream = std::stringstream(row);
			std::for_each(features.begin(), features.end(), [&](std::string &s) {
				its.push_back(inserter(dataTable.at(s), dataTable.at(s).begin()));
			});
			while (getline(rowStream, word, delim)) {
				// converting column based table (each feature in a column) to a row based table (each feature in a row)
				its[(colIndex++) % length] = stof(word);
			}
		}
	}
	data.close();
}

// returning all the data of a given feature
const std::vector<float> &TimeSeries::getFeatureData(const std::string &feature) const {
	return dataTable.at(feature);
}

// returning all the features names
const std::vector<std::string> &TimeSeries::getFeatureNames() const {
	return features;
}

// returning the time step data
const std::vector<float> &TimeSeries::getTimeSteps() const {
	return dataTable.at(features[timeStepsIndex]);
}
