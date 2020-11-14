

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <vector>
#include <string>
#include <map>

class TimeSeries {
	std::map<std::string, std::vector<float>> dataTable;
	std::vector<std::string> features;
	int timeStepsIndex;
	
	/**
	* gets a csv and extract the data from him to the class use
	* @param csvFileName - a csv file
	*/
	void createTable(char const* csvFileName);

public:
	
	/**
	* create a new Time series
	* @param CSVfileName - the Time series data : the csv should have headers and a column represent a time step
	* @param timeStepIndex - optional parameter the index to the time step column is the data defaults to 0
	*/
	explicit TimeSeries(const char* CSVfileName);
	
	/**
	* create a new Time series
	* @param CsvFileName - the Time series data : the csv should have headers and a column represent a time step
	* @param timeStepIndex - optional parameter the index to the time step column is the data defaults to 0
	*/
	__attribute__((unused)) TimeSeries(const char* CsvFileName, int timeStepIndex);
	
	/**
	*
	* @return the number of samples in the time series
	*/
	int getNumOfSamples() const;
	
	/**
	*
	* @param feature - a string which is a name of a feature in the data
	* @return  a vector with all the the data of the feature
	*/
	const std::vector<float> &getFeatureData(const std::string &feature) const;
	
	/**
	*
	* @return a vector of all the feature name in the same order as the headers from the csv
	*/
	const std::vector<std::string> &getFeatureNames() const;
	
	/**
	*
	* @return the time data from the time series
	*/
	const std::vector<float> &getTimeSteps() const;
};

#endif /* TIMESERIES_H_ */
