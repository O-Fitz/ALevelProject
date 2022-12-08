#include "Collisions.h"

Projection::Projection(glm::vec2 axis) : axis(axis), min(0), max(0) {
}

Projection::Projection(glm::vec2 axis, double min, double max) : axis(axis), min(min), max(max) {

}

bool Projection::overlap(const Projection& proj2) {
	return (min<=proj2.min<=max) || (min <= proj2.max <= max);
}