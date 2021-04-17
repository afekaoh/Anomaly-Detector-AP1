/*
* Author: Adam Shapira; 3160044809
 */
#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string>

#include <fstream>
#include <sstream>
#include <utility>
#include <vector>
#include "../Anomaly-detection/SimpleAnomalyDetector.h"
//#include "HybridAnomalyDetector.h"
//#include "SimpleAnomalyDetector.h"


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
	DefaultIO* dio;
	std::string description;
public:
	Command(DefaultIO* const &dio, const std::string &description) : dio(dio),
	                                                                 description(std::move(
			                                                                 description)) {}
	
	virtual void execute() = 0;
	
	virtual std::string const &getDescription() {
		return description;
	};
	
	std::string read() const {
		return std::move(dio->read());
	}
	
	void read(float* f) {
		dio->read(f);
	}
	
	void write(const std::string &s) const {
		dio->write(s);
	}
	
	void write(const float &f) const {
		dio->write(f);
	}
	
	virtual ~Command() = default;
};

// an utility to share the anomaly detection data between the commands
class DetectorUtil {
	// the anomaly detector
	AnomalyDetector &_anomalyDetector;
	// the report vector;
	std::vector<AnomalyReport> _report;
	// the report condensed to time windows
	std::vector<std::pair<long, long>> _fileReport;
	// number of data points in the detect time series
	long size;
public:
	explicit DetectorUtil(AnomalyDetector &anomalyDetector) : _anomalyDetector(
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
		write("Please upload your local train CSV file.\n");
		// uploading the Train CSV
		std::ofstream upld("anomalyTrain.csv");
		std::string row;
		row = read();
		while (row != "done") {
			row += "\n";
			upld.write(row.data(), row.size());
			row = read();
		}
		write("Upload complete.\n");
		upld.close();
		
		// uploading the Test CSV
		write("Please upload your local test CSV file.\n");
		upld = std::ofstream("anomalyTest.csv");
		row = read();
		while (row != "done") {
			row += "\n";
			upld.write(row.data(), row.size());
			row = read();
		}
		
		
		upld.close();
		write("Upload complete.\n");
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
		write("The current correlation threshold is ");
		write(_detector.getThreshold());
		write("\n");
		
		// getting the new threshold
		float th;
		write("Type a new threshold\n");
		read(&th);
		while (0 > th || th > 1) {
			write("please choose a value between 0 and 1.\n");
			read(&th);
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
		
		write("anomaly detection complete.\n");
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
			this->write(std::to_string(ar.timeStep) + "\t" + ar.description + "\n");
		});
		write("done.\n");
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
		auto detectorReport = detector.getFileReport();
		auto userReport = getUserReport();
		// computing positive and negative
		// counting all the reports
		long usrReportSum = 0;
		for_each(userReport.begin(), userReport.end(), [&usrReportSum](std::pair<long, long> &p) {
			usrReportSum += 1 + p.second - p.first;
		});
		int positive = userReport.size();
		
		int negative = detector.getSize() - usrReportSum;
		
		int truePositive = getNumOfIntersections(detectorReport, userReport);
		
		int falsePositive = (int) detectorReport.size() - truePositive;
		
		// getting the true positive rate
		getRate(truePositive, positive, "True Positive Rate: ");
		getRate(falsePositive, negative, "False Positive Rate: ");
		
	}
	
	// returning the number intersection between detector report and the user report
	static int getNumOfIntersections(vector<std::pair<long, long>> &detectorReport,
	                                 vector<std::pair<long, long>> &userReport) {
		int intersectionCounter = 0;
		
		for (auto &uReport : userReport) {
			intersectionCounter += count_if(detectorReport.begin(),
			                                detectorReport.end(),
			                                [&uReport](pair<long, long> &dReport) {
				                                return isIntersect(dReport, uReport);
			                                });
		}
		return intersectionCounter;
	}
	
	void getRate(int numerator, int denominator, std::string const &s) const {
		write(s);
		float rate = (float) numerator / (float) denominator;
		rate = floor(rate * 1000.0f) / 1000.0f;
		write(rate);
		write("\n");
	}
	
	// getting the user report data
	std::vector<std::pair<long, long>> getUserReport() const {
		write("Please upload your local anomalies file.\n");
		std::vector<std::pair<long, long>> userReport;
		std::string row;
		std::string word;
		row = read();
		
		while (row != "done") {
			std::pair<long, long> p;
			std::stringstream sstr(row);
			getline(sstr, word, ',');
			p.first = stol(word);
			getline(sstr, word, ',');
			p.second = stol(word);
			userReport.push_back(p);
			row = read();
		}
		write("Upload complete.\n");
		return std::move(userReport);
	}
	
};

// the main command. responsible to connect the CLI to all the other commends.
class CliCommand : public Command {
	std::map<std::string, Command*> commands;
	DetectorUtil detector;
public:
	CliCommand(DefaultIO* const &dio, AnomalyDetector &anomalyDetector) :
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
			write("Welcome to the Anomaly Detection Server.\nPlease choose an option:\n");
			std::for_each(commands.begin(),
			              commands.end(),
			              [this](const std::pair<std::string, Command*> &c) {
				              this->write(c.first + "." + c.second->getDescription() + "\n");
			              }
			);
			write(st);
			std::string s = read();
			// no need for commend to exit.
			if (s == "6")
				break;
			// validating the input
			while (commands.find(s) == commands.end()) {
				write("enter a valid option:\n");
				s = read();
			}
			commands.at(s)->execute();
		}
	}
	
	// deconstructor
	~CliCommand() override {
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
