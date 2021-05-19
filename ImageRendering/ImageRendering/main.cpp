#include "vector3d.h"
#include "WorkWithBMP.h"
#include "triangle.h"
#include "projectionPlane.h"
#include "intersectionChecker.h"
#include "fileReader.h"
#include "Rtree.h"

using namespace std;

int main()
{
	vector<triangle> triangles =  fileReader::readObj("cow.obj");
	Rtree treeOfModel;
	for(int i=0;i<triangles.size();i++)
	{
		treeOfModel.insert(triangles[i]);
	}
	
	//cout << triangles[triangles.size()-1].getVertex1().getX();
	vector3d cameraPosition(0, -1, 0);
	vector3d cameraDirection(0, 1, 0);
	//triangle triangleOnScene({10, 4, 0}, {10, -4, 0}, {10, 0, 4});
	int widthOfScreen=300, heightOfScreen=300;
	int counter=0;
	projectionPlane plane(widthOfScreen, heightOfScreen, cameraPosition+cameraDirection);
	for(int i=0;i<heightOfScreen;i++)
	{
		for(int j=0;j<widthOfScreen;j++)
		{
			bool wasIntersection=false;
			vector3d intersectionPoint, directionOfRay=plane.getPixelsCoordinatesInWorld()[i][j]-cameraPosition;
			for(int k=0;k<triangles.size();k++)
			{
				wasIntersection=intersectionChecker::rayIntersectsTriangle(cameraPosition, directionOfRay, &triangles[k], intersectionPoint);
				if(wasIntersection==true)
				{
					cout<<"was! "<<counter<<" K: "<<k<<" i: "<<i<<" j: "<<j<<endl;
					counter++;
					break;
				}
			}
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