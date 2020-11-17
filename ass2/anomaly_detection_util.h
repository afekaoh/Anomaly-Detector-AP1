/*
 * animaly_detection_util.h
 *
 * Author: Adam Shapira; 3160044809
 */
#ifndef ANOMALYDETECTORUTIL_H_
#define ANOMALYDETECTORUTIL_H_


#include <vector>
#include <memory>

// utility functions that can be used
namespace detect_util {
	template<typename Iterator, typename Functor>
	void for_each_2(Iterator begin1, Iterator begin2, const Iterator &end1, const Iterator &end2, const Functor &f) {
		while (begin1 != end1 && begin2 != end2) {
			f(*begin1, *begin2);
			begin1++;
			begin2++;
		}
	}
}

/**
*
* @param x - a vector of floats
* @return the average of all the numbers in x
*/
float avg(const std::vector<float> &x);

/**
*
* @param x - a vector of floats
* @return the variance of all the numbers in x
*/
float var(const std::vector<float> &x);

/**
*
* @param x - a vector of floats
* @param y - a vector of floats
* @return the covariance of x and y
*/
float cov(const std::vector<float> &x, const std::vector<float> &y);


/**
*
* @param x - a vector of floats
* @param y - a vector of floats
* @return the correlation between the values of x and y
*/
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

/**
*
* @param points - a vector of points
*
* the function calculate the liner regression of the points
*
* @return Line represent the liner regression of the points
*/
Line linear_reg(std::vector<std::unique_ptr<Point>> const &points);

// returns the deviation between point p and a group of points
float dev(const Point &p, std::vector<std::unique_ptr<Point>> &points);

// returns the deviation between point p and a group of points
float dev(const Point &p, Point** points, int size);

// returns the deviation between point p and the line
float dev(const Point &p, const Line &l);

#endif
