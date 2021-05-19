#include "vector3d.h"
#include "WorkWithBMP.h"
#include "triangle.h"
#include "projectionPlane.h"
#include "intersectionChecker.h"
#include "fileReader.h"
#include "rtree.h"

using namespace std;

int main()
{
	vector<triangle> triangles =  fileReader::readObj("cow.obj");
	//cout << triangles[0].getVertex1().getX() << " " << triangles[0].getVertex1().getY() << " " << triangles[0].getVertex1().getZ() << endl;
	//cout << triangles[0].getVertex2().getX() << " " << triangles[0].getVertex2().getY() << " " << triangles[0].getVertex2().getZ() << endl;
	//cout << triangles[0].getVertex3().getX() << " " << triangles[0].getVertex3().getY() << " " << triangles[0].getVertex3().getZ() << endl;
	vector3d normal = triangles[0].getNormal();
	//cout << normal.getX() << normal.getY() << normal.getZ();
	rtree treeOfModel;
	//for(int i=0;i<triangles.size();i++)
	//{
	//	treeOfModel.insert(triangles[i]);
	//}
	//cout << triangles[triangles.size()-1].getVertex1().getX();
	vector3d cameraPosition(0, -1, 0);
	vector3d cameraDirection(0, 1, 0);
	vector3d lightPosition(0, 1,0 );
	//triangle triangleOnScene({10, 4, 0}, {10, -4, 0}, {10, 0, 4});
	int widthOfScreen = 1200, heightOfScreen = 1200;
	int counter=0;
	projectionPlane plane(widthOfScreen, heightOfScreen, cameraPosition+cameraDirection);
	for(int i=0;i<heightOfScreen;i++)
	{
		for(int j=0;j<widthOfScreen;j++)
		{
			bool wasIntersection=false;
			vector3d intersectionPoint, directionOfRay=plane.getPixelsCoordinatesInWorld()[i][j]-cameraPosition;
			int k = 0;
			for(int k=0;k<triangles.size();k++)
			{
				wasIntersection=intersectionChecker::rayIntersectsTriangle(cameraPosition, directionOfRay, &triangles[k], intersectionPoint);
				if(wasIntersection==true)
				{
					//cout<<"was! "<<counter<<" K: "<<k<<" i: "<<i<<" j: "<<j<<endl;
					counter++;
					break;
				}
			}
			if(wasIntersection==true)
			{
				vector3d lightRay = intersectionPoint - lightPosition;
				double cosAlpha = lightRay.findCos(triangles[k].getNormal());
				//cout << "alpha:" << cosAlpha;
				plane.pixels[i][j].B = abs(cosAlpha)* 255;
				plane.pixels[i][j].G= abs(cosAlpha) * 255;
				plane.pixels[i][j].R= abs(cosAlpha) * 255;
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