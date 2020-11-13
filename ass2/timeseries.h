

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <vector>
#include <string>
#include <map>

using namespace std;

class TimeSeries {
	std::map<std::string, std::vector<float>> dataTable;
	std::vector<std::string> features;
	int timeStepsIndex;

public:
	explicit TimeSeries(const char* CSVfileName);
	
	explicit TimeSeries(const char* CSVfileName, int timeStepIndex);
	
	int getNumOfSamples() const;
	
	vector<float> getFeatureData(const std::string &feature) const;
	
	vector<string> getFeatureNames() const;
	
	void createTable(char const* csvFileName);
	
	const std::vector<float> getTimeSteps() const;
};

#endif /* TIMESERIES_H_ */
