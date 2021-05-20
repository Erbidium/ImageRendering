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


double triangle ::getX_max()
{
	double Max_x = this->vertex1.getX();
	if (Max_x < this->vertex2.getX())
	{
		Max_x = this->vertex2.getX();
	}
	if (Max_x < this->vertex3.getX())
	{
		Max_x = this->vertex3.getX();
	}
	return Max_x;
}

double triangle::getY_max()
{
	double Max_y = this->vertex1.getY();
	if (Max_y < this->vertex2.getY())
	{
		Max_y = this->vertex2.getY();
	}
	if (Max_y < this->vertex3.getY())
	{
		Max_y = this->vertex3.getY();
	}
	return Max_y;
}

double triangle::getZ_max()
{
	double Max_z = this->vertex1.getZ();
	if (Max_z < this->vertex2.getZ())
	{
		Max_z = this->vertex2.getZ();
	}
	if (Max_z < this->vertex3.getZ())
	{
		Max_z = this->vertex3.getZ();
	}
	return Max_z;
}

double triangle::getX_min()
{
	double Min_x = this->vertex1.getX();
	if (Min_x > this->vertex2.getX())
	{
		Min_x = this->vertex2.getX();
	}
	if (Min_x > this->vertex3.getX())
	{
		Min_x = this->vertex3.getX();
	}
	return Min_x;
}

double triangle::getY_min()
{
	double Min_y = this->vertex1.getY();
	if (Min_y > this->vertex2.getY())
	{
		Min_y = this->vertex2.getY();
	}
	if (Min_y > this->vertex3.getY())
	{
		Min_y = this->vertex3.getY();
	}
	return Min_y;
}

double triangle::getZ_min()
{
	double Min_z = this->vertex1.getZ();
	if (Min_z > this->vertex2.getZ())
	{
		Min_z = this->vertex2.getZ();
	}
	if (Min_z > this->vertex3.getZ())
	{
		Min_z = this->vertex3.getZ();
	}
	return Min_z;
}