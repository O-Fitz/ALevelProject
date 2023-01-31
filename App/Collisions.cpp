#include "Collisions.h"
#include <iostream>

Projection::Projection(glm::vec2 axis) : axis(axis), min(0), max(0) {
}

Projection::Projection(glm::vec2 axis, double min, double max) : axis(axis), min(min), max(max) {

}

bool Projection::seperate(const Projection& proj2) {
	return (min > proj2.max || max < proj2.min);
}