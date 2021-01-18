/*
 * animaly_detection_util.cpp
 *
 * Author: Adam Shapira; 3160044809
 */

#include <cmath>
#include "anomaly_detection_util.h"
#include <algorithm>
#include <numeric>
#include <vector>
#include <memory>


namespace detect_util {
	float squaresAvg(const std::vector<float> &x);
}

// finds the average of a given array
float avg(const std::vector<float> &x) {
	
	float sum = std::accumulate(x.begin(), x.end(), 0.0f);
	
	return sum / x.size();
}

// finds the average of the squares of the array
float detect_util::squaresAvg(const std::vector<float> &x) {
	std::vector<float> sqrX;
	std::for_each(x.begin(), x.end(), [&sqrX](const float &num) {
		sqrX.push_back(num * num);
	});
	return avg(sqrX);
}

// returns the variance of X
float var(const std::vector<float> &x) {
	auto const average = avg(x);
	float squareAvr = average * average;
	return detect_util::squaresAvg(x) - squareAvr;
}


// returns the covariance of X and Y
float cov(const std::vector<float> &x, const std::vector<float> &y) {
	
	float avgX = avg(x);
	float avgY = avg(y);
	std::vector<float> cov;
	
	detect_util::for_each_2(x.begin(),
	                        y.begin(),
	                        x.end(),
	                        y.end(),
	                        [&cov, &avgX, &avgY](const float &xVal, const float &yVal) {
		                        auto diff = (xVal - avgX) * (yVal - avgY);
		                        cov.push_back(diff);
	                        });
	
	return avg(cov);
}


// returns the Pearson correlation coefficient of X and Y
float pearson(const std::vector<float> &x, const std::vector<float> &y) {
	return cov(x, y) / (std::sqrt(var(x) * var(y)));
}

// performs a linear regression and returns the line equation
Line linear_reg(std::vector<std::unique_ptr<Point>> const &points) {
	std::vector<float> x;
	std::vector<float> y;
	std::for_each(points.begin(), points.end(), [&x, &y](const std::unique_ptr<Point> &p) {
		x.push_back(p->x);
		y.push_back(p->y);
	});
	float a = cov(x, y) / var(x);
	float b = avg(y) - a * avg(x);
	return {a, b};
}

// returns the deviation between point p and the line equation of the points
float dev(const Point &p, Point** points, int size) {
	std::vector<std::unique_ptr<Point>> ps;
	std::for_each(points, points + size, [&ps](Point* p) {
		ps.push_back(std::unique_ptr<Point>(p));
	});
	return dev(p, linear_reg(ps));
}

float dev(const Point &p, std::vector<std::unique_ptr<Point>> &points) {
	return dev(p, linear_reg(points));
}

// returns the deviation between point p and the line
float dev(const Point &p, const Line &l) {
	return std::fabs(l.f(p.x) - p.y);
}




