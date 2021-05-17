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
	triangle triangleOnScene({3, 4, 0}, {3, -4, 0}, {3, 0, 4});
	int widthOfScreen=50, heightOfScreen=50;
	projectionPlane plane(widthOfScreen, heightOfScreen, cameraPosition+cameraDirection);
	for(int i=0;i<heightOfScreen;i++)
	{
		for(int j=0;j<widthOfScreen;j++)
		{
			vector3d intersectionPoint;
			vector3d directionOfRay=plane.getPixelsCoordinatesInWorld()[i][j]-cameraPosition;
			bool wasIntersection=intersectionChecker::rayIntersectsTriangle(cameraPosition, directionOfRay, &triangleOnScene, intersectionPoint);
			if(wasIntersection==true)
				cout<<"was!"<<endl;
				//cout<<'X';
			//else
			//	cout<<'-';
		}
		cout<<endl;
	}
}