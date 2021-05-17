#include <iostream>
#include "vector3d.h"
#include "triangle.h"
#include "projectionPlane.h"
#include "intersectionChecker.h"

using namespace std;

int main()
{
	vector3d cameraPosition(0, 5, 0);
	vector3d cameraDirection(0, -1, 0);
	triangle triangleOnScene({ -4, 0, -1 }, { 2, 0, -1 }, { 0, 0, 4 });
	int widthOfScreen=100, heightOfScreen=100;
	projectionPlane plane(widthOfScreen, heightOfScreen, cameraPosition+cameraDirection);
	for(int i=0;i<heightOfScreen;i++)
	{
		for(int j=0;j<widthOfScreen;j++)
		{
			vector3d intersectionPoint;
			vector3d directionOfRay=plane.getPixelsCoordinatesInWorld()[i][j]-cameraPosition;
			bool wasIntersection=intersectionChecker::rayIntersectsTriangle(cameraPosition, directionOfRay, &triangleOnScene, intersectionPoint);
			if(wasIntersection==true)
				cout<<'X';
			else
				cout<<'-';
		}
		cout<<endl;
	}
}