#include "Collisions.h"

Projection::Projection(Vector axis) : axis(axis), min(0), max(0) {
}

Projection::Projection(Vector axis, double min, double max) : axis(axis), min(min), max(max) {

}