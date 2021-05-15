#include "vector3d.h"

vector3d::vector3d():
x(0), y(0), z(0)
{}

vector3d::vector3d(double newX, double newY, double newZ):
x(newX), y(newY), z(newZ)
{}

double vector3d::getX() const
{
	return x;
}

double vector3d::getY() const
{
	return y;
}

double vector3d::getZ() const
{
	return z;
}

void vector3d::setX(double newX)
{
	x = newX;
}

void vector3d::setY(double newY)
{
	y = newY;
}

void vector3d::setZ(double newZ)
{
	z = newZ;
}

vector3d vector3d::operator+(vector3d a) const
{
	return {x + a.getX(), y + a.getY(), z + a.getZ()};
}

vector3d vector3d::operator-(vector3d a) const
{
	return {x - a.getX(), y - a.getY(), z - a.getZ()};
}

double vector3d::operator*(vector3d a) const
{
	return x * a.getX() + y * a.getY() + z * a.getZ();
}

vector3d vector3d::crossProduct(vector3d a) const
{
	return {y * a.getZ() - z * a.getY(), z * a.getX() - x * a.getZ(), x * a.getY() - y * a.getX()};
}
