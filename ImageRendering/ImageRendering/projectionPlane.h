#pragma once
#include <vector>
#include "vector3d.h"
#include "PIXEL.h"

class projectionPlane
{
	std::vector<std::vector<vector3d>> pixelsCoordinatesInWorld;
	int width;
	int height;
	vector3d centerOfPlane;
public:
	std::vector<std::vector<PIXEL>> pixels;
	std::vector<std::vector<vector3d>> getPixelsCoordinatesInWorld(){return pixelsCoordinatesInWorld;}
	projectionPlane(int width, int height, vector3d centerOfPlane):
	width(width), height(height), centerOfPlane(centerOfPlane),
	pixels(height, std::vector<PIXEL>(width)), pixelsCoordinatesInWorld(height, std::vector<vector3d>(width))
	{
		for(int pixelY=0;pixelY<height;pixelY++)
		{
			for(int pixelX=0;pixelX<width;pixelX++)
			{
				double pixelNDCX=(pixelX+0.5)/width;
				double pixelNDCY=(pixelY+0.5)/height;
				double pixelScreenX=2*pixelNDCX-1;
				double pixelScreenY=1-2*pixelNDCY;
				vector3d relativePointOfPixel(pixelScreenX, pixelScreenY, 0);
				pixelsCoordinatesInWorld[pixelY][pixelX]=relativePointOfPixel+centerOfPlane;
			}
		}
	}
};

