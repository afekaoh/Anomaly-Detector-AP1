

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <vector>
#include <string>
#include <map>

using namespace std;

class TimeSeries {
	std::map<std::string,std::vector<float>> dataTable;
	std::vector<std::string> features;

public:
	explicit TimeSeries(const char* CSVfileName);
	
	int getNumOfSamples() const;
	
	vector<float> getFeatureData(const std::string &feature) const;
	
	vector<string> getFeatureNames() const;
	
	void createTable(char const* csvFileName);
};

#endif /* TIMESERIES_H_ */
