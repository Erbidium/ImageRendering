#include "vector3d.h"
#include "WorkWithBMP.h"
#include "triangle.h"
#include "projectionPlane.h"
#include "intersectionChecker.h"
#include "fileReader.h"
#include "Rtree.h"
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

int main()
{
	clock_t start_time = clock();
	vector<triangle> triangles =  fileReader::readObj("car.obj");
	Rtree treeOfModel;
	for(int i=0;i<triangles.size();i++)
	{
		treeOfModel.insert(triangles[i]);
	}
	cout<<"Tree build! Current time: "<< (double)(clock() - start_time) / CLOCKS_PER_SEC << " seconds." <<endl;

	/*scene 1 settings
	vector3d cameraPosition(2, -2, 2);
	vector3d lookAtPoint(0, 0, 0);
	vector3d cameraDirection=lookAtPoint-cameraPosition;
	vector3d lightPosition(0.5, -1.5, 3);
	int widthOfScreen=720, heightOfScreen=480;
	*/

	vector3d cameraPosition(0, -30, 0);
	vector3d lookAtPoint(0, 0, 0);
	vector3d cameraDirection=lookAtPoint-cameraPosition;
	vector3d lightPosition(0.5, -1.5, 3);
	int widthOfScreen=720, heightOfScreen=480;
	
	projectionPlane plane(widthOfScreen, heightOfScreen, cameraPosition+cameraDirection, cameraDirection, cameraPosition);
	vector<vector<vector3d>> pixelsInWorld=plane.getPixelsCoordinatesInWorld();
	vector3d intersectionPoint;
	vector3d directionOfRay;
	triangle intersectedTriangle;
	for(int i=0;i<heightOfScreen;i++)
	{
		for(int j=0;j<widthOfScreen;j++)
		{
			directionOfRay=pixelsInWorld[i][j]-cameraPosition;
			bool wasIntersection=treeOfModel.intersectionOfRayAnd3Dmodel(cameraPosition, directionOfRay, intersectionPoint, intersectedTriangle);
			
			if(wasIntersection==true)
			{
				if (!treeOfModel.findIntersectionLigthInTree(lightPosition,intersectionPoint - lightPosition, treeOfModel.getRoot(), intersectionPoint, intersectedTriangle))
				{
					vector3d lightRay = intersectionPoint - lightPosition;
					double cosAlpha = lightRay.findCos(intersectedTriangle.getNormal());
					plane.pixels[i][j].B = abs(cosAlpha) * 255;
					plane.pixels[i][j].G = abs(cosAlpha) * 255;
					plane.pixels[i][j].R = abs(cosAlpha) * 255;
				}
				else
				{
					plane.pixels[i][j].B = 0;
					plane.pixels[i][j].G = 0;
					plane.pixels[i][j].R = 0;
				}
			}
			else
			{
				plane.pixels[i][j].B=30;
				plane.pixels[i][j].G=30;
				plane.pixels[i][j].R=30;
			}
		}
	}
	WorkWithBMP::createBMPImage(heightOfScreen, widthOfScreen, plane.pixels);
	clock_t end_time = clock();
	cout << "\nImage rendered! Current time: " << (double)(end_time - start_time) / CLOCKS_PER_SEC << " seconds." << endl;
}