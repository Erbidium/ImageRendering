#pragma once
#include "node.h"
#include "vector3d.h"
#include "triangle.h"

class intersectionChecker
{
public:
	static bool rayIntersectsTriangle(vector3d rayOrigin, vector3d rayVector, triangle* inTriangle, vector3d& outIntersectionPoint); 
	static bool intersectionRayAndBox(vector3d ray, vector3d rayOrigin, node*current);
};
