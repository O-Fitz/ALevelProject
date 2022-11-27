#include "Vector.h"
#include <math.h>

Vector::Vector() {
	x = 0;
	y = 0;
}

Vector::Vector(double x, double y) : x(x), y(y) {
}

Vector::Vector(double items[2]) : x(items[0]), y(items[1]) {
}

Vector Vector::operator+(const Vector& other) {
	return Vector(this->x + other.x, this->y + other.y);
}
Vector Vector::operator-(const Vector& other) {
	return Vector(this->x - other.x, this->y - other.y);
}
Vector Vector::operator*(const double& scale) {
	return Vector(this->x * scale, this->y * scale);
}
Vector Vector::operator/(const double& scale) {
	return Vector(this->x / scale, this->y / scale);
}
Vector Vector::operator+=(const Vector& other) {
	Vector n = copy() + other;
	x = n.x;
	y = n.y;
	return n;
}
Vector Vector::operator-=(const Vector& other) {
	Vector n = copy() - other;
	x = n.x;
	y = n.y;
	return n;
}
Vector Vector::operator*=(const double& scale) {
	Vector n = copy() * scale;
	x = n.x;
	y = n.y;
	return n;
}
Vector Vector::operator/=(const double& scale) {
	Vector n = copy() / scale;
	x = n.x;
	y = n.y;
	return n;
}

Vector Vector::operator=(const Vector& other) {
	this->x = other.x;
	this->y = other.y;
	return other;
}

Vector Vector::norm() {
	return copy()/abs();
}

double Vector::abs() {
	return sqrt(x * x + y * y);
}

double Vector::dot(const Vector& other) {
	return (x * other.x) + (y * other.y);
}

double Vector::angle(const Vector& other) {
	return acos(dot(other)/abs());
}

Vector Vector::copy() {
	return Vector(this->x, this->y);
}

Vector Vector::perp(Vector other) {
	return other - par(other);
}

Vector Vector::par(Vector other) {
	Vector cop = copy();
	return cop * dot(other) / dot(cop);
}

double dot2(const Vector& vec1, const Vector& vec2) {
	return (vec1.x * vec2.x) + (vec1.y * vec2.y);
}