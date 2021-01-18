

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string>

#include <fstream>
#include <sstream>
#include <utility>
#include <vector>
#include "../Anomaly-detection/HybridAnomalyDetector.h"
//#include "HybridAnomalyDetector.h"


class DefaultIO {
public:
	virtual std::string read() = 0;
	
	virtual void write(std::string text) = 0;
	
	virtual void write(float f) = 0;
	
	virtual void read(float* f) = 0;
	
	virtual ~DefaultIO() {}
	
	// you may add additional methods here
};


// you may add here helper classes


class Command {
protected:
	DefaultIO* dio;
	std::string description;
public:
	Command(DefaultIO* const &dio, std::string description) : dio(dio),
	                                                          description(std::move(description)) {}
	
	virtual void execute() = 0;
	
	virtual std::string const &getDescription() {
		return description;
	};
	
	virtual ~Command() {}
};

// an utility to share the anomaly detection data between the commands
class DetectorUtil {
	// the anomaly detector
	HybridAnomalyDetector _anomalyDetector;
	// the report vector;
	std::vector<AnomalyReport> _report;
	// the report condensed to time windows
	std::vector<std::pair<long, long>> _fileReport;
	// number of data points in the detect time series
	long size;
public:
	explicit DetectorUtil(HybridAnomalyDetector &anomalyDetector) : _anomalyDetector(
			anomalyDetector) {
		_report = std::vector<AnomalyReport>();
		size = 0;
	}
	
	// set the threshold for the anomaly detector
	void setThreshold(float threshold) {
		this->_anomalyDetector.setCorrelationThreshold(threshold);
	}
	
	// getting the current threshold
	float getThreshold() const {
		return this->_anomalyDetector.getCorrelationThreshold();
	}
	
	// applying the learn algorithm to the given time series
	void learn(TimeSeries const &ts) {
		this->_anomalyDetector.learnNormal(ts);
	}
	
	// applying the learn algorithm to the given time series
	void detect(TimeSeries const &ts) {
		_report = this->_anomalyDetector.detect(ts);
		// getting the size of the time series
		size = ts.getTimeSteps().size();
	}
	
	// getting the raw report from the anomaly detector
	std::vector<AnomalyReport> const &getReport() {
		return _report;
	}
	
	// getting the condensed report
	std::vector<std::pair<long, long>> const &getFileReport() const {
		return _fileReport;
	}
	
	// setting a new condensed report
	void setFileReport(std::vector<std::pair<long, long>> const &fileReport) {
		DetectorUtil::_fileReport = fileReport;
	}
	
	// getting the size
	long getSize() const {
		return size;
	}
	
};

// upload the CSV files
class UploadCommand : public Command {
public:
	void execute() override {
		dio->write("Please upload your local train CSV file.\n");
		std::ofstream upld;
		// uploading the Train CSV
		upld = std::ofstream("anomalyTrain.csv");
		std::string row;
		row = dio->read();
		while (row != "done") {
			row += "\n";
			upld.write(row.data(), row.size());
			row = dio->read();
		}
		dio->write("Upload complete.\n");
		
		// uploading the Test CSV
		dio->write("Please upload your local test CSV file.\n");
		upld = std::ofstream("anomalyTest.csv");
		row = dio->read();
		while (row != "done") {
			row += "\n";
			upld.write(row.data(), row.size());
			row = dio->read();
		}
		
		upld.close();
		dio->write("Upload complete.\n");
	}
	
	UploadCommand(DefaultIO* const &dio, const std::string &description) : Command(dio,
	                                                                               description) {}
};

/**
* a command for changing the anomaly detection settings
* right now support only threshold changing
**/
class SettingsCommand : public Command {
	DetectorUtil &_detector;
public:
	
	SettingsCommand(DefaultIO* const &dio, std::string const &description,
	                DetectorUtil &detector) : Command(dio, description), _detector(detector) {
	}
	
	void execute() override {
		// displaying the current threshold
		dio->write("The current correlation threshold is ");
		dio->write(_detector.getThreshold());
		dio->write("\n");
		
		// getting the new threshold
		float th;
		dio->write("Type a new threshold\n");
		dio->read(&th);
		while (0 > th || th > 1) {
			dio->write("please choose a value between 0 and 1.\n");
			dio->read(&th);
		}
		
		// updating the util
		_detector.setThreshold(th);
	}
};

// a commend to applaying the anomaly detection algorithm to the uploaded CSV files
class LearnCommand : public Command {
	DetectorUtil &detector;
public:
	LearnCommand(DefaultIO* const &dio, std::string const &description,
	             DetectorUtil &detector) : Command(dio, description), detector(detector) {}
	
	void execute() override {
		// setting the time series
		const auto &train = TimeSeries("anomalyTrain.csv");
		auto const &test = TimeSeries("anomalyTest.csv");
		
		// applying the algorithms
		detector.learn(train);
		detector.detect(test);
		// crating the condensed report
		unifyReport();
		
		dio->write("anomaly detection complete.\n");
	}
	
	// create a condensed report and updating the util
	void unifyReport() {
		auto report = detector.getReport();
		std::pair<long, long> p;
		std::vector<std::pair<long, long>> fileReport;
		for (auto ar = report.begin(); ar != report.end(); ar++) {
			long ts = ar->timeStep;
			const std::string &des = ar->description;
			if (ar == report.begin() || des != (ar - 1)->description ||
			    ts != (ar - 1)->timeStep + 1) {
				// if the anomaly report has different description or the difference more than 1
				// from his predecessor then it's a new report sequence
				p.first = ts;
			}
			if (ar == report.end() - 1 || des != (ar + 1)->description ||
			    ts != (ar + 1)->timeStep - 1) {
				// if the anomaly report has different description or the difference more than 1
				// from his successor then it's an end of a report sequence
				p.second = ts;
				fileReport.push_back(p);
			}
		}
		// updating the util
		detector.setFileReport(fileReport);
	}
};

// a command to get the anomaly report
class DetactCommand : public Command {
	DetectorUtil &detector;
public:
	DetactCommand(DefaultIO* const &dio, std::string const &description,
	              DetectorUtil &detector) :
			Command(dio, description), detector(detector) {}
	
	void execute() override {
		auto report = detector.getReport();
		// writing the report to the dio
		for_each(report.begin(), report.end(), [this](const AnomalyReport &ar) {
			this->dio->write(std::to_string(ar.timeStep) + "\t" + ar.description + "\n");
		});
		dio->write("done.\n");
	}
};

// a command to analyze the the detection algorithm
class AnalyzeCommand : public Command {
	DetectorUtil &detector;
public:
	AnalyzeCommand(DefaultIO* const &dio, std::string const &description,
	               DetectorUtil &detector) :
			Command(dio, description), detector(detector) {}
	
	// checking if 2 ranges of <low, high> are intersected
	static bool isIntersect(std::pair<long, long> &pair, std::pair<long, long> &pair1) {
		return pair.second >= pair1.first && pair.first <= pair1.second;
	}
	
	void execute() override {
		// getting all the necessary data
		auto fileReport = detector.getFileReport();
		auto userReport = getUserReport();
		auto falsePositiveVec = fileReport;
		// computing positive and negative
		int positive = userReport.size();
		// counting all the reports
		long sum = 0;
		for_each(userReport.begin(), userReport.end(), [&sum](std::pair<long, long> &p) {
			sum += 1 + p.second - p.first;
		});
		int negative = detector.getSize() - sum;
		
		int truePositive = 0;
		// computing the true positive and getting the false negative reports
		for_each(fileReport.begin(), fileReport.end(),
		         [&userReport, &falsePositiveVec, &truePositive](std::pair<long, long> &rp) {
			         for_each(userReport.begin(), userReport.end(),
			                  [&rp, &falsePositiveVec, &truePositive](std::pair<long, long> &up) {
				                  if (isIntersect(rp, up)) {
					                  int size = falsePositiveVec.size();
					                  // removing all the reports that have intersection
					                  falsePositiveVec.erase(std::remove(falsePositiveVec.begin(),
					                                                     falsePositiveVec.end(),
					                                                     rp
					                                         ),
					                                         falsePositiveVec.end());
					                  // adding the reports removed to the true positive count
					                  truePositive += size - falsePositiveVec.size();
					
				                  }
			                  });
		         }
		
		);
		
		int FP = falsePositiveVec.size();
		// getting the true positive rate
		dio->write("True Positive Rate: ");
		float TPR = (float) truePositive / (float) positive;
		TPR = round(TPR * 1000.0f) / 1000.0f;
		dio->write(TPR);
		dio->write("\n");
		
		// getting the false alarm rate
		dio->write("False Positive Rate: ");
		float FAR = (float) FP / ((float) negative);
		FAR = floor(FAR * 1000.0f) / 1000.0f;
		dio->write(FAR);
		dio->write("\n");
	}
	
	// getting the user report data
	std::vector<std::pair<long, long>> getUserReport() {
		dio->write("Please upload your local anomalies file.\n");
		std::vector<std::pair<long, long>> userReport;
		std::string row;
		std::string word;
		row = dio->read();
		std::stringstream sstr;
		
		while (row != "done") {
			std::pair<long, long> p;
			sstr = std::stringstream(row);
			getline(sstr, word, ',');
			p.first = stol(word);
			getline(sstr, word, ',');
			p.second = stol(word);
			userReport.push_back(p);
			row = dio->read();
		}
		dio->write("Upload complete.\n");
		return std::move(userReport);
	}
	
};

// the main command. responsible to connect the CLI to all the other commends.
class CliCommand : public Command {
	std::map<std::string, Command*> commands;
	DetectorUtil detector;
public:
	CliCommand(DefaultIO* const &dio, HybridAnomalyDetector anomalyDetector) :
			Command(dio, ""), detector(anomalyDetector) {
		// crating all the commends
		commands = std::map<std::string, Command*>();
		commands.emplace("1", new UploadCommand(dio, "upload a time series csv file"));
		commands.emplace("2", new SettingsCommand(dio, "algorithm settings",
		                                          this->detector));
		commands.emplace("3",
		                 new LearnCommand(dio, "detect anomalies", this->detector));
		commands.emplace("4",
		                 new DetactCommand(dio, "display results", this->detector));
		commands.emplace("5",
		                 new AnalyzeCommand(dio, "upload anomalies and analyze results",
		                                    this->detector));
	}
	
	void execute() override {
		std::string st = "6.exit\n";
		while (true) {
			dio->write("Welcome to the Anomaly Detection Server.\nPlease choose an option:\n");
			std::for_each(commands.begin(),
			              commands.end(),
			              [this](const std::pair<std::string, Command*> &c) {
				              this->dio->write(c.first + "." + c.second->getDescription() + "\n");
			              }
			);
			dio->write(st);
			std::string s = dio->read();
			// no need for commend to exit.
			if (s == "6")
				break;
			// validating the input
			while (commands.find(s) == commands.end()) {
				dio->write("enter a valid option:\n");
				s = dio->read();
			}
			commands.at(s)->execute();
		}
	}
	
	// deconstractor
	~CliCommand() {
		// deleting all the commends
		std::for_each(commands.begin(),
		              commands.end(),
		              [](const std::pair<std::string, Command*> &c) {
			              delete c.second;
		              }
		);
	}
	
};


#endif /* COMMANDS_H_ */
