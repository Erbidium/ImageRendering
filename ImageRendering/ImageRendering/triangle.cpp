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
