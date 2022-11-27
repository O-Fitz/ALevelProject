#pragma once

class Vector {
private:
	double items[2] = {0, 0};


public:
	double& x = items[0];
	double& y = items[1];

	Vector();
	Vector(double x, double y);
	Vector(double items[2]);

	Vector operator+(const Vector& other);
	Vector operator-(const Vector& other);
	Vector operator*(const double& scale);
	Vector operator/(const double& scale);
	Vector operator+=(const Vector& other);
	Vector operator-=(const Vector& other);
	Vector operator*=(const double& scale);
	Vector operator/=(const double& scale);
	Vector operator=(const Vector& other);

	Vector norm();
	double abs();

	double dot(const Vector& other);
	double angle(const Vector& other);
	Vector perp(Vector other);
	Vector par(Vector other);

	Vector copy();

};

double dot2(const Vector& vec1, const Vector& vec2);