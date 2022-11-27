#pragma once
#include "Maths/Vector.h"

struct Projection {
	double min;
	double max;
	Vector axis;

	Projection(Vector axis);
	Projection(Vector axis, double min, double max);
};

