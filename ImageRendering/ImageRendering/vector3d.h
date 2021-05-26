#pragma once
#include <iostream>
using namespace std;
class vector3d
{
	double x;
	double y;
	double z;
public:
	vector3d();
	vector3d(double newX, double newY, double newZ);
	double getX() const;
	double getY() const;
	double getZ() const;
	void setX(double newX);
	void setY(double newY);
	void setZ(double newZ);
	vector3d operator+(vector3d a) const;
	vector3d operator-(vector3d a) const;
	double operator*(vector3d a) const;
	vector3d operator*(double number)const;
	bool operator<=(vector3d a)const;
	bool operator>=(vector3d a)const;
	vector3d crossProduct(vector3d a) const;
	double getLength();
	double findCos(vector3d b);
	bool operator==(vector3d second);
	void print() {
		cout << "X: " << x << "Y: " << y << "Z: " << z << endl;
	}
	vector3d operator /(double num);
};
