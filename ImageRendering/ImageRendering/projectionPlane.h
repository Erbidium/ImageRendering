#pragma once
#include <vector>
#include "vector3d.h"
#include "pixel.h"

class projectionPlane
{
	std::vector<std::vector<vector3d>> pixelsCoordinatesInWorld;
	int width;
	int height;
	vector3d centerOfPlane;
	std::vector<std::vector<PIXEL>> pixels;
public:
	projectionPlane(int width, int height, vector3d centerOfPlane, vector3d cameraDirection, vector3d cameraPosition);
	std::vector<std::vector<vector3d>> getPixelsCoordinatesInWorld()const;
	std::vector<std::vector<PIXEL>> getPixelMatrix()const;
	void setPixelMatrix(std::vector<std::vector<PIXEL>> &newPixels);
};

