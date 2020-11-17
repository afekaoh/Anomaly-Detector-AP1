

#include <iostream>
#include <vector>
#include <algorithm>
#include "AnomalyDetector.h"
#include "SimpleAnomalyDetector.h"
#include <fstream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>

/* time */

using namespace std;

int samples = 100;

// this is a simple test to put you on the right track
void generateTrainCSV(float a1, float b1, float a2, float b2) {
	ofstream out("trainFile1.csv");
	out << "A,B,C,D" << endl;
	Line ac(a1, b1);
	Line bd(a2, b2);
	for (int i = 1; i <= samples; i++) {
		float a = i;
		float b = rand() % 40;
		out << a << "," << b << "," << ac.f(a) - (0.02 * 1) + (rand() % (40 * 1)) / 100.0f << ","
		    << bd.f(b) - (0.02 * 1) + (rand() % (40 * 1)) / 100.0f << endl;
	}
	out.close();
}

void generateTestCSV(float a1, float b1, float a2, float b2, int anomaly, int i1) {
	ofstream out("testFile1.csv");
	out << "A,B,C,D" << endl;
	Line ac(a1, b1);
	Line bd(a2, b2);
	for (int i = 1; i <= samples; i++) {
		float a = i;
		float b = rand() % 40;
		if (i == anomaly || i == i1) {
			out << a << "," << b << "," << ac.f(a) << ","
			    << bd.f(b) + 1
			    << endl;
		} else
			out << a << "," << b << "," << ac.f(a) - (0.02 * 1) + (rand() % (40 * 1)) / 100.0f << ","
			    << bd.f(b) - (0.02 * 1) + (rand() % (40 * 1)) / 100.0f << endl;
	}
	out.close();
}

void checkCorrelationTrain(correlatedFeatures c, string f1, string f2, float a, float b) {
	if (c.feature1 == f1) {
		if (c.feature2 != f2)
			cout << "wrong correlated feature of " << f1 << " (-20)" << endl;
		else {
			if (c.corrlation < 0.99)
				cout << f1 << "-" << f2 << " wrong correlation detected (-5)" << endl;
			if (c.lin_reg.a < a - 0.5f || c.lin_reg.a > a + 0.5f)
				cout << f1 << "-" << f2 << " wrong value of line_reg.a (-5)" << endl;
			if (c.lin_reg.b < b - 0.5f || c.lin_reg.b > b + 0.5f)
				cout << f1 << "-" << f2 << " wrong value of line_reg.b (-5)" << endl;
			if (c.threshold > 0.3)
				cout << f1 << "-" << f2 << " wrong threshold detected (-5)" << endl;
		}
	}
	
}


int main() {
	std::size_t learnTime = 0;
	std::size_t detectTime = 0;
	ofstream out("avg.csv");
	int n = 100;
	for (int j = 1; j < 10; j++) {
		for (int i = 0; i < n; i++) {
			bool flag = true;
			srand((i + 1) * time(NULL));
			float a1 = 1 + rand() % 50, b1 = -50 + rand() % 100;
			float a2 = 1 + rand() % 20, b2 = -50 + rand() % 100;
			
			
			// test the learned model: (40 points)
			// expected correlations:
			//	A-C: y=a1*x+b1
			//	B-D: y=a2*x+b2
			
			generateTrainCSV(a1, b1, a2, b2);
			TimeSeries ts("trainFile1.csv");
			
			
			SimpleAnomalyDetector ad;
			
			ad.learnNormal(ts);
			vector<correlatedFeatures> cf = ad.getNormalModel();
			
			if (cf.size() != 2)
				cout << "wrong size of correlated features (-40)" << endl;
			else
				for_each(cf.begin(), cf.end(), [&a1, &b1, &a2, &b2](const correlatedFeatures &c) {
					checkCorrelationTrain(c, "A", "C", a1, b1); // 20 points
					checkCorrelationTrain(c, "B", "D", a2, b2); // 20 points
				});
			
			// test the anomaly detector: (60 points)
			// one simply anomaly is injected to the data
			int anomaly = 5 + rand() % 90; // one anomaly injected in a random time step
			int anomaly1 = 5 + rand() % 90; // one anomaly injected in a random time step
			generateTestCSV(a1, b1, a2, b2, anomaly, anomaly1);
			TimeSeries ts2("testFile1.csv");
			
			vector<AnomalyReport> r = ad.detect(ts2);

//
//		printCF(cf, std::cout);

//		bool anomlyDetected = false;
			int anomlyDetected = 0;
			int falseAlarms = 0;
			for_each(r.begin(), r.end(),
			         [&anomaly, &anomaly1, &anomlyDetected, &falseAlarms, &cf, &flag, &i](const AnomalyReport &ar) {
				         if (ar.description == "B-D" && (ar.timeStep == anomaly || ar.timeStep == anomaly1)) {
					         anomlyDetected++;
				         } else {
					         cout << anomaly << " " << ar.timeStep << " " << anomaly1 << endl;
					         falseAlarms++;
				         }
			         });
			
			if (!anomlyDetected)
				cout << "the anomaly was not detected (-30) " << samples << endl;
			
			if (falseAlarms > 0)
				cout << "you have " << falseAlarms << " false alarms (-" << min(30, falseAlarms * 3) << ")" << endl;
		}
	}
	cout << "done" << endl;
	return 0;
}

