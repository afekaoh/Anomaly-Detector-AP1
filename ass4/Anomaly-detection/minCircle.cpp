#include "minCircle.h"
#include <cmath>

/**
*
* \param p1 - the first point
* \param p2 - the second point
* \return - the distance squared between them
*/
float distanceSquare(const Point &p1, const Point &p2) {
	float diffX = p2.x - p1.x;
	float diffY = p2.y - p1.y;
	return (diffX * diffX) + (diffY * diffY);
}

/**
* \param circle - the circle to check whether the point is in
* \param point  - the point to check if it is in the circle
* \return - if the point is inside the circle
*/
bool isIn(const Circle &circle, const Point &point) {
	return distanceSquare(circle.center, point) <= circle.radius;
}

/**
* \param boundary - an array of points on the circumference of the circle
* \param size - the size of the array
* \return the circle that made with this points on it's circumference
*
* \note - the function returns the circle with squared radius for time optimization
*/
Circle trivialCircle(Point* boundary, uint_fast8_t size) {
	switch (size) {
		case 1: {
			return {boundary[0], 0};
		}
		case 2: {
			Point center((boundary[0].x + boundary[1].x) / 2,
			             (boundary[0].y + boundary[1].y) / 2);
			return {center, (distanceSquare(center, boundary[0]))};
		}
		case 3: {
			Point a = boundary[0];
			Point b = boundary[1];
			Point c = boundary[2];
			
			float slopeAB = (b.y - a.y) / (b.x - a.x);
			float slopeBC = (c.y - b.y) / (c.x - b.x);
			float centerX = (slopeAB * slopeBC * (a.y - c.y) + slopeBC * (a.x + b.x) -
			                 slopeAB * (b.x + c.x)) /
			                (2 * (slopeBC - slopeAB));
			float centerY = -(1 / slopeAB) * (centerX - ((a.x + b.x) / 2)) + (a.y + b.y) / 2;
			Point center(centerX, centerY);
			return {center, (distanceSquare(center, a))};
		}
		default: {
			return {Point(), 0};
		}
	}
}

/**
* \brief - we decided to implement the welzl algorithm as it's shown in https://en.wikipedia.org/wiki/Smallest-circle_problem
* \param points - the points to find the minimum circle that enclose them
* \param pointsSize - the size of the points array
* \param boundary - an array of points that located on the circumference of the min circle
* \param boundarySize - the size of the boundary array
* \return   the function returns the minimum enclosing array for the points given
*
* \note for time optimization purposes the function returns the circle with it's array squared
*/
Circle
welzl(std::unique_ptr<struct Point, std::default_delete<struct Point>>* points, size_t pointsSize,
      Point* boundary, uint_fast8_t boundarySize) {
	// we implements the welzl algorithm
	if (pointsSize == 0 || boundarySize == 3) {
		return trivialCircle(boundary, boundarySize);
	}
	const auto &p = *points[pointsSize - 1];
	const auto &circle = welzl(points, pointsSize - 1, boundary, boundarySize);
	if (isIn(circle, p)) {
		return circle;
	}
	boundary[boundarySize] = p;
	return welzl(points, pointsSize - 1, boundary, boundarySize + 1);
}

/**
*
* \param points - the points to find the minimum circle enclosing them
* \param size - the size of the array
* \return the minimum circle enclosing all the points
*/
Circle findMinCircle(std::unique_ptr<Point, std::default_delete<Point>>* points, size_t size) {
	Point p(0, 0);
	Point boundary[3] = {p, p, p};
	Circle c = welzl(points, size, boundary, 0);
	c.radius = sqrt(c.radius);
	return c;
}

float distance(Point const &p1, Point const &p2) {
	return sqrt(distanceSquare(p1, p2));
}
