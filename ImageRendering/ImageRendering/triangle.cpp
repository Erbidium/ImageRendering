#include "triangle.h"

vector3d triangle::getVerticle1() const
{
	return verticle1;
}

vector3d triangle::getVerticle2() const
{
	return verticle2;
}

vector3d triangle::getVerticle3() const
{
	return verticle3;
}

void triangle::setVerticle1(vector3d newVerticle1)
{
	verticle1 = newVerticle1;
}

void triangle::setVerticle2(vector3d newVerticle2)
{
	verticle1 = newVerticle2;
}

void triangle::setVerticle3(vector3d newVerticle3)
{
	verticle1 = newVerticle3;
}
