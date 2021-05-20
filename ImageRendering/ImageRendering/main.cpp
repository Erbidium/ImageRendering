#include "vector3d.h"
#include "WorkWithBMP.h"
#include "triangle.h"
#include "projectionPlane.h"
#include "intersectionChecker.h"
#include "fileReader.h"
#include "Rtree.h"
#include <iostream>

using namespace std;

int triangle::index = 0;

int main()
{
	vector<triangle> triangles =  fileReader::readObj("cow.obj");
	Rtree treeOfModel;
	for(int i=0;i<triangles.size();i++)
	{
		treeOfModel.insert(triangles[i]);
	}
	cout<<"number of Triangles: "<<treeOfModel.count()<<endl;
	//cout << triangles[triangles.size()-1].getVertex1().getX();
	vector3d cameraPosition(0, -1, 0);
	vector3d cameraDirection(0, 1, 0);
	//triangle triangleOnScene({10, 4, 0}, {10, -4, 0}, {10, 0, 4});
	int widthOfScreen=100, heightOfScreen=100;
	int counter=0;
	projectionPlane plane(widthOfScreen, heightOfScreen, cameraPosition+cameraDirection);
	for(int i=0;i<heightOfScreen;i++)
	{
		for(int j=0;j<widthOfScreen;j++)
		{
			vector3d intersectionPoint, directionOfRay=plane.getPixelsCoordinatesInWorld()[i][j]-cameraPosition;
			bool wasIntersection=treeOfModel.intersectionOfRayAnd3Dmodel(cameraPosition, directionOfRay, intersectionPoint);
			if(wasIntersection==true)
			{
				plane.pixels[i][j].B=255;
				plane.pixels[i][j].G=255;
				plane.pixels[i][j].R=255;
			}
			else
			{
				plane.pixels[i][j].B=0;
				plane.pixels[i][j].G=0;
				plane.pixels[i][j].R=0;
			}
		}
	}
	WorkWithBMP::createBMPImage(heightOfScreen, widthOfScreen, plane.pixels);
}