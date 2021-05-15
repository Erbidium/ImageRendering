#pragma once
#include "vector3d.h"

class triangle
{
	vector3d vertex1;
	vector3d vertex2;
	vector3d vertex3;
public:
	vector3d getVertex1() const;
	vector3d getVertex2() const;
	vector3d getVertex3() const;
	void setVertex1(vector3d);
	void setVertex2(vector3d);
	void setVertex3(vector3d);
};
