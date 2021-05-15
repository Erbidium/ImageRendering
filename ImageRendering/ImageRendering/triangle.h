#pragma once
#include "vector3d.h"

class triangle
{
	vector3d verticle1;
	vector3d verticle2;
	vector3d verticle3;
public:
	vector3d getVerticle1() const;
	vector3d getVerticle2() const;
	vector3d getVerticle3() const;
	void setVerticle1(vector3d newVerticle1);
	void setVerticle2(vector3d newVerticle2);
	void setVerticle3(vector3d newVerticle3);
};
