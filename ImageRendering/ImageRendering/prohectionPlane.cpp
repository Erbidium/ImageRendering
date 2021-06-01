#include "projectionPlane.h"

projectionPlane::projectionPlane(int width, int height, vector3d centerOfPlane, vector3d cameraDirection, vector3d cameraPosition):
width(width), height(height), centerOfPlane(centerOfPlane),
pixels(height, std::vector<PIXEL>(width)), pixelsCoordinatesInWorld(height, std::vector<vector3d>(width))
{
	vector3d cameraDirectionNormalized=cameraDirection/(cameraDirection.getLength());
	vector3d rightVector=vector3d(0, 1, 0).crossProduct(cameraDirectionNormalized);
	//if(rightVector==vector3d{0, 0, 0})
	//{
	//	rightVector=vector3d(0, 1, 0).crossProduct(cameraDirectionNormalized);
	//}
	vector3d upVector=cameraDirectionNormalized.crossProduct(rightVector);
	double imageAspectRatio=static_cast<double>(width)/height;
	for(int pixelY=0;pixelY<height;pixelY++)
	{
		for(int pixelX=0;pixelX<width;pixelX++)
		{
			pixelsCoordinatesInWorld[pixelY][pixelX]=cameraDirection+cameraPosition+rightVector*(static_cast<double>(2*pixelX)/width-1)*imageAspectRatio+upVector*(static_cast<double>(2*pixelY)/height-1);
		}
	}
}

std::vector<std::vector<vector3d>> projectionPlane::getPixelsCoordinatesInWorld()const
{
	return pixelsCoordinatesInWorld;
}

void projectionPlane::setPixelMatrix(std::vector<std::vector<PIXEL>> &newPixels)
{
	pixels=newPixels;
}


std::vector<std::vector<PIXEL>> projectionPlane::getPixelMatrix()const
{
	return pixels;
}
