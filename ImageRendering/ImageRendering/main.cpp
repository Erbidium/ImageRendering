#include <iostream>
#include "vector3d.h"
#include "triangle.h"
#include "projectionPlane.h"
#include "intersectionChecker.h"

using namespace std;

int main()
{
	vector3d cameraPosition(0, 0, 0);
	vector3d cameraDirection(1, 0, 0);
	triangle triangleOnScene({10, 4, 0}, {10, -4, 0}, {10, 0, 4});
	int widthOfScreen=100, heightOfScreen=100;
	projectionPlane plane(widthOfScreen, heightOfScreen, cameraPosition+cameraDirection);
	for(int i=0;i<heightOfScreen;i++)
	{
		for(int j=0;j<widthOfScreen;j++)
		{
			//cout<<plane.getPixelsCoordinatesInWorld()[i][j].getX()<<" ";
			//cout<<plane.getPixelsCoordinatesInWorld()[i][j].getY()<<" ";
			//cout<<plane.getPixelsCoordinatesInWorld()[i][j].getZ()<<endl;
			vector3d intersectionPoint;
			vector3d directionOfRay=plane.getPixelsCoordinatesInWorld()[i][j]-cameraPosition;
			bool wasIntersection=intersectionChecker::rayIntersectsTriangle(cameraPosition, directionOfRay, &triangleOnScene, intersectionPoint);
			if(wasIntersection==true)
				//cout<<"was!"<<endl;
				cout<<'X';
			else
				cout<<'-';
		}
		cout<<endl;
	}
}