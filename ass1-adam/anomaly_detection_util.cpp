/*
 * animaly_detection_util.cpp
 *
 * Author: Adam Shapira; 3160044809
 */

#include <math.h>
#include "anomaly_detection_util.h"

// finds the average of a given array
float avg(float* x, int size) {
	if (size == 0)
		return 0;
	float sum = 0;
	for (int i = 0; i < size; i++) {
		sum += *x;
		x++;
	}
	return sum / (float) size;
}

// finds the average of the squares of the array
static float squaresAvg(float* x, int size) {
	float newX[size];
	for (int i = 0; i < size; i++) {
		newX[i] = pow(x[i], 2);
	}
	return avg(newX, size);
}

// returns the variance of X and Y
float var(float* x, int size) {
	float squareAvr = pow(avg(x, size), 2);
	return squaresAvg(x, size) - squareAvr;
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size) {
	float covar[size];
	float avgX = avg(x, size);
	float avgY = avg(y, size);
	for (int i = 0; i < size; i++) {
		covar[i] = (x[i] - avgX) * (y[i] - avgY);
	}
	return avg(covar, size);
}


// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size) {
	float sigX = sqrt(var(x, size));
	float sigY = sqrt(var(y, size));
	return cov(x, y, size) / (sigX * sigY);
}

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size) {
	float x[size];
	float y[size];
	for (int i = 0; i < size; i++) {
		x[i] = points[i]->x;
		y[i] = points[i]->y;
	}
	float a = cov(x, y, size) / var(x, size);
	float b = avg(y, size) - a * avg(x, size);
	return Line(a, b);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p, Point** points, int size) {
	return dev(p, linear_reg(points, size));
}

// returns the deviation between point p and the line
float dev(Point p, Line l) {
	return fabs(l.f(p.x) - p.y);;
}




