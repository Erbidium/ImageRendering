#include "triangle.h"

triangle::triangle(vector3d newVertex1, vector3d newVertex2, vector3d newVertex3):
vertex1(newVertex1),vertex2(newVertex2),vertex3(newVertex3)
{
	
}

vector3d triangle::getVertex1() const
{
	return vertex1;
}

vector3d triangle::getVertex2() const
{
	return vertex2;
}

vector3d triangle::getVertex3() const
{
	return vertex3;
}

void triangle::setVertex1(vector3d newVertex1)
{
	vertex1 = newVertex1;
}

void triangle::setVertex2(vector3d newVertex2)
{
	vertex2 = newVertex2;
}

void triangle::setVertex3(vector3d newVertex3)
{
	vertex3 = newVertex3;
}

vector3d triangle::getCenter()
{
	vector3d center;
	center.setX((vertex1.getX()+vertex2.getX()+vertex3.getX())/3);
	center.setY((vertex1.getY()+vertex2.getY()+vertex3.getY())/3);
	center.setZ((vertex1.getZ()+vertex2.getZ()+vertex3.getZ())/3);
	return center;
}

vector3d triangle::getNormal()
{
	vector3d vector1 = vertex2 - vertex1;
	vector3d vector2 = vertex3 - vertex1;
	vector3d normal = vector1.crossProduct(vector2);
	return normal;
}

