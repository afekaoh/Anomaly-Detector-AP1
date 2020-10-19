/*
 * animaly_detection_util.cpp
 *
 * Author: write your ID and name here
 */

#include <math.h>
#include <iostream>
#include "anomaly_detection_util.h"

float avg(float *x, int size) {
	float sum;
	for (int i = 0; i < size; ++i) {
		sum += x[i];
	}
	return sum / (float) size;
}

// returns the variance of X and Y
float var(float *x, int size) {
	float average = avg(x, size);
	float varianceSum;
	for (int i = 0; i < size; ++i) {
		varianceSum += powf(x[i] - average, 2);
	}
	return varianceSum / (float) size;
}

// returns the covariance of X and Y
float cov(float *x, float *y, int size) {
	float averageX = avg(x, size);
	float averageY = avg(y, size);
	float covariances[size];
	for (int i = 0; i < size; ++i) {
		covariances[i] = (x[i] - averageX) * (y[i] - averageY);
	}
	float covarianceSum;
	for (int i = 0; i < size; ++i) {
		covarianceSum += covariances[i];
	}
	return covarianceSum / (float) size;
}

void printArr(float *x, int size) {
	for (int i = 0; i < size; ++i) {
		std::cout << x[i] << " ";
	}
	std::cout << std::endl;
}

// returns the Pearson correlation coefficient of X and Y
float pearson(float *x, float *y, int size) {
	return (cov(x, y, size) / (sqrt(var(x, size) * var(y, size))));
}


// performs a linear regression and returns the line equation
Line linear_reg(Point **points, int size) {

	return Line(0, 0);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p, Point **points, int size) {
	return 0;
}

// returns the deviation between point p and the line
float dev(Point p, Line l) {
	return 0;
}




