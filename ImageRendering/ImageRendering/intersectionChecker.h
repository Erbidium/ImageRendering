#pragma once
#include "vector3d.h"
#include "triangle.h"

class intersectionChecker
{
public:
	static bool rayIntersectsTriangle(vector3d rayOrigin, vector3d rayVector, triangle* inTriangle, vector3d& outIntersectionPoint); 
};
