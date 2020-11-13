/*
 * animaly_detection_util.h
 *
 * Author: Adam Shapira; 3160044809
 */
#ifndef ANOMALYDETECTORUTIL_H_
#define ANOMALYDETECTORUTIL_H_


#include <vector>
#include <memory>

float avg(const std::vector<float> &x);

// returns the variance of X
float var(const std::vector<float> &x);

// returns the covariance of X and Y
float cov(const std::vector<float> &x, const std::vector<float> &y);


// returns the Pearson correlation coefficient of X and Y
float pearson(const std::vector<float> &x, const std::vector<float> &y);

class Line {
public:
	float a, b;
	
	Line() : a(0), b(0) {};
	
	Line(float a, float b) : a(a), b(b) {}
	
	float f(float x) const {
		return a * x + b;
	}
};

class Point {
public:
	const float x, y;
	
	Point(float x, float y) : x(x), y(y) {}
};

// performs a linear regression and returns the line equation
Line linear_reg(std::vector<std::unique_ptr<Point>> const &points);

// returns the deviation between point p and a group of points
float dev(const Point &p, std::vector<std::unique_ptr<Point>> &points);

// returns the deviation between point p and a group of points
float dev(const Point &p, Point** points, int size);

// returns the deviation between point p and the line
float dev(const Point &p, const Line &l);

#endif
