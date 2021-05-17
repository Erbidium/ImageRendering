#include "vector3d.h"
#include "WorkWithBMP.h"
#include "triangle.h"
#include "projectionPlane.h"
#include "intersectionChecker.h"
//#include "fileReader.h"

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
	//vector<triangle> triangles =  fileReader::readObj("cow.obj");
	//cout << triangles[triangles.size()-1].getVertex1().getX();
}