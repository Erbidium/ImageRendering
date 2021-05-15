#pragma once

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
	vector3d crossProduct(vector3d a) const;
};
