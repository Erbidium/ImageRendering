#pragma once
#include <iostream>

#include "triangle.h"
#include <vector>

struct Node
{
	Node():x_max(INT_MIN),x_min(INT_MAX),y_max(INT_MIN),y_min(INT_MAX), z_max(INT_MIN), z_min(INT_MAX){}
	~Node()
	{
		for(int i=0;i<childs.size();i++)
		{
			delete childs[i];
		}
	}
	Node* operator=(Node* current)
	{
		//std::cout<<"operator called\n";
		childs=current->childs;
		triangles=current->triangles;
		x_max=current->x_max;
		x_min=current->x_min;

		y_max=current->y_max;
		y_min=current->y_min;
		
		z_max=current->z_max;
		z_min=current->z_min;
		return this;
	}
	void setDefaultRect()
	{
		x_max=INT_MIN;
		x_min=INT_MAX;

		y_max=INT_MIN;
		y_min=INT_MAX;
		
		z_max=INT_MIN;
		z_min=INT_MAX;
	}
	vector3d getBoxCenter()
	{
		return { (x_max + x_min) / 2, (y_max + y_min) / 2, (z_max + z_min) / 2 };
	}
	///replace INT_MAX with bool statements for initialized or not 3d rectangles!!!!
	///INT_MIN for x_max and others!!!
	std::vector<Node*> childs;
	std::vector<triangle> triangles;
	double x_max, x_min, y_max, y_min, z_max, z_min;
};