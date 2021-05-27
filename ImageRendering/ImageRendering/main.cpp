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
#include <cmath>

using namespace std;

void takeParameters(vector3d& camaraPosition, vector3d& lookAtPoint, vector3d& lightPosition, int& widthOfScreen,int& heightOfScreen, int& intens, vector3d& lightColour, vector3d& lightModel);

int main()
{
	vector3d cameraPosition, lookAtPoint, lightPosition, lightColour, lightModel;
	int widthOfScreen, heightOfScreen, intens;
	takeParameters(cameraPosition, lookAtPoint, lightPosition, widthOfScreen, heightOfScreen, intens, lightColour, lightModel);
	clock_t start_time = clock();
	vector<triangle> triangles =  fileReader::readObj("car.obj");
	Rtree treeOfModel;
	for(int i=0;i<triangles.size();i++)
	{
		treeOfModel.insert(triangles[i]);
	}
	cout<<"\nTree build! Tree building time: "<< (double)(clock() - start_time) / CLOCKS_PER_SEC << " seconds." <<endl;

	/*scene 1 settings
	vector3d cameraPosition(2, -2, 2);
	vector3d lookAtPoint(0, 0, 0);
	vector3d cameraDirection=lookAtPoint-cameraPosition;
	vector3d lightPosition(0.5, -1.5, 3);
	int widthOfScreen=720, heightOfScreen=480;
	*/
	/*
	vector3d cameraPosition(-20, 15, 20);
	vector3d lookAtPoint(0, 0, 0);
	vector3d cameraDirection=lookAtPoint-cameraPosition;
	cameraDirection=cameraDirection/(cameraDirection.getLength());
	vector3d lightPosition(-30, 20, 25);
	int widthOfScreen=720, heightOfScreen=480;
	*/
	vector3d  cameraDirection = (lookAtPoint - cameraPosition) / ((lookAtPoint - cameraPosition).getLength());
	projectionPlane plane(widthOfScreen, heightOfScreen, cameraPosition+cameraDirection, cameraDirection, cameraPosition);
	vector<vector<vector3d>> pixelsInWorld=plane.getPixelsCoordinatesInWorld();
	vector3d intersectionPoint;
	vector3d directionOfRay;
	triangle intersectedTriangle;
	clock_t start_time2 = clock();
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
					plane.pixels[i][j].B = min((double)255, lightColour.getZ() * lightModel.getZ() * abs(cosAlpha) * 255 * intens / pow((lightPosition - intersectionPoint).getLength(), 2));
					plane.pixels[i][j].G = min((double)255, lightColour.getY() * lightModel.getY() * abs(cosAlpha) * 255 * intens / pow((lightPosition - intersectionPoint).getLength(), 2));
					plane.pixels[i][j].R = min((double)255, lightColour.getX() * lightModel.getX() * abs(cosAlpha) * 255 * intens / pow((lightPosition - intersectionPoint).getLength(), 2));
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
	cout << "\nImage rendered! Render time: " <<(end_time - start_time2)<< "seconds\nTotal time: "<< (double)(end_time - start_time) / CLOCKS_PER_SEC << " seconds." << endl;
}

void takeParameters(vector3d& camaraPosition, vector3d& lookAtPoint, vector3d& lightPosition, int& widthOfScreen, int& heightOfScreen, int& intens, vector3d& lightColour, vector3d& lightModel)
{
	double temp;
	cout << "\nEnter camera position:\nx: ";
	cin >> temp;
	camaraPosition.setX(temp);
	cout << "\ny: ";
	cin >> temp;
	camaraPosition.setY(temp);
	cout << "\nz: ";
	cin >> temp;
	camaraPosition.setZ(temp);
	cout << "\nEnter the point where you want to look:\nx: ";
	cin >> temp;
	lookAtPoint.setX(temp);
	cout << "\ny: ";
	cin >> temp;
	lookAtPoint.setY(temp);
	cout << "\nz: ";
	cin >> temp;
	lookAtPoint.setZ(temp);
	cout << "\nEnter the light source position:\nx: ";
	cin >> temp;
	lightPosition.setX(temp);
	cout << "\ny: ";
	cin >> temp;
	lightPosition.setY(temp);
	cout << "\nz: ";
	cin >> temp;
	lightPosition.setZ(temp);
	cout << "\nEnter light intense:\n";
	cin >> intens;
	cout << "\nEnter the colour of the light:\nR: ";
	cin >> temp;
	lightColour.setX(temp/255);
	cout << "\nG: ";
	cin >> temp;
	lightColour.setY(temp/255);
	cout << "\nB: ";
	cin >> temp;
	lightColour.setZ(temp/255);
	cout << "\nEnter the colour of the model:\nR: ";
	cin >> temp;
	lightModel.setX(temp/255);
	cout << "\nG: ";
	cin >> temp;
	lightModel.setY(temp/255);
	cout << "\nB: ";
	cin >> temp;
	lightModel.setZ(temp/255);
	cout << "\nEnter width of image:\n";
	cin >> widthOfScreen;
	cout << "\nEnter height of image:\n";
	cin >> heightOfScreen;
}