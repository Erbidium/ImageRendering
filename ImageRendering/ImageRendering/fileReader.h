#pragma once
#include "triangle.h"
#include <vector>
#include <string>

class fileReader
{
public:
	static std::vector<triangle>readObj(std::string nameOfFile);
	static void takeParameters(string& pathToFile, vector3d& cameraPosition, vector3d& lookAtPoint, vector<vector3d>& lightPosition, int& widthOfScreen,int& heightOfScreen, vector<int>& intens, vector<vector3d>& lightColour, vector3d& lightModel);
};

