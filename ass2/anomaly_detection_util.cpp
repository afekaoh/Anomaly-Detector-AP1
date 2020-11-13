/*
 * animaly_detection_util.cpp
 *
 * Author: Adam Shapira; 3160044809
 */

#include <cmath>
#include "anomaly_detection_util.h"
#include <stdexcept>
#include <algorithm>

// finds the average of a given array
float avg(const float* x, int size) {
	if (size <= 0)
		throw std::invalid_argument("size invalid");
	
	float sum = 0;
	std::for_each(x, x + size, [&sum](const float &num) {
		sum += num;
	});
	return sum / (float) size;
}

// finds the average of the squares of the array
static float squaresAvg(const float* x, int size) {
	if (size <= 0)
		throw std::invalid_argument("size invalid");
	
	float newX[size];
	for (int i = 0; i < size; i++) {
		newX[i] = x[i] * x[i];
	}
	return avg(newX, size);
}

// returns the variance of X
float var(const float* x, int size) {
	if (size <= 0)
		throw std::invalid_argument("size invalid");
	
	auto const average = avg(x, size);
	float squareAvr = average * average;
	return squaresAvg(x, size) - squareAvr;
}

// returns the covariance of X and Y
float cov(const float* x, const float* y, int size) {
	if (size <= 0)
		throw std::invalid_argument("size invalid");
	float covar[size];
	float avgX = avg(x, size);
	float avgY = avg(y, size);
	for (int i = 0; i < size; i++) {
		covar[i] = (x[i] - avgX) * (y[i] - avgY);
	}
	return avg(covar, size);
}


// returns the Pearson correlation coefficient of X and Y
float pearson(const float* x, const float* y, int size) {
	if (size <= 0)
		throw std::invalid_argument("size invalid");
	
	return cov(x, y, size) / (std::sqrt(var(x, size) * var(y, size)));
}

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size) {
	if (size <= 0)
		throw std::invalid_argument("size invalid");
	
	float x[size];
	float y[size];
	for (int i = 0; i < size; i++) {
		x[i] = points[i]->x;
		y[i] = points[i]->y;
	}
	float a = cov(x, y, size) / var(x, size);
	float b = avg(y, size) - a * avg(x, size);
	return {a, b};
}

// returns the deviation between point p and the line equation of the points
float dev(Point p, Point** points, int size) {
	return dev(p, linear_reg(points, size));
}

// returns the deviation between point p and the line
float dev(Point p, Line l) {
	return std::fabs(l.f(p.x) - p.y);
}




