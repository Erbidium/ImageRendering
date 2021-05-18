#pragma once
#include "Rtree.h"
#include "triangle.h"
#include <vector>

struct Node
{
	std::vector<Node*> childs;
	std::vector<triangle> triangles;
	double x_max, x_min, y_max, y_min, z_max, z_min;
};