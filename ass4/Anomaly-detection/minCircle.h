
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "anomaly_detection_util.h"


class Circle {
public:
	Point center;
	float radius;
	
	Circle(Point c, float r) : center(c), radius(r) {}
	
	Circle() : center(Point()), radius(0) {}
};

Circle findMinCircle(std::unique_ptr<Point, std::default_delete<Point>>* points, size_t size);

float distance(Point const &p1, Point const &p2);

#endif /* MINCIRCLE_H_ */
