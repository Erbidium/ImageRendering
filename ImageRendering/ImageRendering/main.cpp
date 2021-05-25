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
/*
bool operator==(triangle a, triangle b)
{
	if(((a.getVertex1().getX()==b.getVertex1().getX())&&(a.getVertex1().getY()==b.getVertex1().getY())&&(a.getVertex1().getZ()==b.getVertex1().getZ()))&&
       ((a.getVertex2().getX()==b.getVertex2().getX())&&(a.getVertex2().getY()==b.getVertex2().getY())&&(a.getVertex2().getZ()==b.getVertex2().getZ()))&&
	   ((a.getVertex3().getX()==b.getVertex3().getX())&&(a.getVertex3().getY()==b.getVertex3().getY())&&(a.getVertex3().getZ()==b.getVertex3().getZ()))
		)
		return true;
	else
		return false;
}*/

using namespace std;

int main()
{
	clock_t start_time = clock();
	vector<triangle> triangles =  fileReader::readObj("cow.obj");
	Rtree treeOfModel;
	for(int i=0;i<triangles.size();i++)
	{
		treeOfModel.insert(triangles[i]);
	}
	cout<<"build!"<<endl;
	vector<triangle> gatheredTriangles;
	cout<<"number of Triangles: "<<treeOfModel.count(gatheredTriangles)<<endl;
	cout<<"trigs size: "<<gatheredTriangles.size()<<endl;/*
	for(int i=0;i<gatheredTriangles.size()*1;i++)
	{
		for(int j=i+1;j<gatheredTriangles.size();j++)
		{
			if(gatheredTriangles[i]==gatheredTriangles[j])
			{
				cout<<"Yes1"<<endl;
				break;
			}
		}
	}*/
	/*ofstream outFile("out.txt");
	for(int i=0;i<gatheredTriangles.size();i++)
	{
		triangle trig=gatheredTriangles[i];
		outFile<<trig.getVertex1().getX()<<" "<<trig.getVertex1().getY()<<" "<<trig.getVertex1().getZ()<<endl;
		outFile<<trig.getVertex2().getX()<<" "<<trig.getVertex2().getY()<<" "<<trig.getVertex2().getZ()<<endl;
		outFile<<trig.getVertex3().getX()<<" "<<trig.getVertex3().getY()<<" "<<trig.getVertex3().getZ()<<endl;
		outFile<<endl;
	}*/
	//cout << triangles[triangles.size()-1].getVertex1().getX();
	vector3d cameraPosition(0, -1, 0);
	vector3d cameraDirection(0, 1, 0);
	vector3d lightPosition(0, 1,0 );
	//triangle triangleOnScene({10, 4, 0}, {10, -4, 0}, {10, 0, 4});
	int widthOfScreen=1500, heightOfScreen=1500;
	int counter=0;
	projectionPlane plane(widthOfScreen, heightOfScreen, cameraPosition+cameraDirection);
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
				vector3d lightRay = intersectionPoint - lightPosition;
				double cosAlpha = lightRay.findCos(intersectedTriangle.getNormal());
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
	clock_t end_time = clock();
	cout << "\nTime: " << (double)(end_time - start_time) / CLOCKS_PER_SEC << endl;
}