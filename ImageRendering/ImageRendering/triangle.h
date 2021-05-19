#pragma once
#include "vector3d.h"

class triangle
{
	vector3d vertex1;
	vector3d vertex2;
	vector3d vertex3;
public:
	triangle(vector3d, vector3d, vector3d);
	triangle(){}
	vector3d getVertex1() const;
	vector3d getVertex2() const;
	vector3d getVertex3() const;
	void setVertex1(vector3d);
	void setVertex2(vector3d);
	void setVertex3(vector3d);
	vector3d getCenter();
	double getX_max();
	double getY_max();
	double getZ_max();
	double getX_min();
	double getY_min();
	double getZ_min();
};