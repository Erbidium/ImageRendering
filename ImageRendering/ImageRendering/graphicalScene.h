#pragma once
#include <vector>
#include "vector3d.h"
#include "fileReader.h"
#include "rtree.h"
#include "projectionPlane.h"

class graphicalScene
{
	vector3d cameraPosition;
	vector3d lookAtPoint;
	vector3d  cameraDirection;
	vector3d lightModel;
	vector<vector3d>  lightPosition;
	vector<vector3d>  lightColour;
	vector<int> lightIntesities;
	int widthOfScreen;
	int heightOfScreen;
	string pathToObjFile;
	rtree treeOfModel;
	clock_t start_time;
	clock_t start_renderTime;
	projectionPlane * plane;
	double angle;
public:
	graphicalScene();
	void buildTree();
	void renderPixels();
	void pixelsToFile()const;
	~graphicalScene();
};
