#include "vector3d.h"
#include "workWithBMP.h"
#include "triangle.h"
#include "projectionPlane.h"
#include "fileReader.h"
#include "rtree.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>
#include <fstream>

using namespace std;

void takeParameters(string& pathToFile, vector3d& cameraPosition, vector3d& lookAtPoint, vector<vector3d>& lightPosition, int& widthOfScreen,int& heightOfScreen, vector<int>& intens, vector<vector3d>& lightColour, vector3d& lightModel);

int main()
{
	vector3d cameraPosition, lookAtPoint, lightModel;
	vector<vector3d>  lightPosition, lightColour;
	int widthOfScreen, heightOfScreen;
	string pathToObjFile;
	vector<int> lightIntesities;
	takeParameters(pathToObjFile, cameraPosition, lookAtPoint, lightPosition, widthOfScreen, heightOfScreen, lightIntesities, lightColour, lightModel);
	clock_t start_time = clock();
	vector<triangle> triangles =  fileReader::readObj(pathToObjFile);
	rtree treeOfModel;
	for(int i=0;i<triangles.size();i++)
	{
		treeOfModel.insert(triangles[i]);
	}
	cout<<"\nTree build! Tree building time: "<< (double)(clock() - start_time) / CLOCKS_PER_SEC << " seconds." <<endl;
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
				int R=0, G=0, B=0;
				for(int k=0;k<lightPosition.size();k++)
				{
					if (!treeOfModel.findIntersectionLigthInTree(lightPosition[k],intersectionPoint - lightPosition[k], treeOfModel.getRoot(), intersectionPoint, intersectedTriangle))
					{
						vector3d lightRay = intersectionPoint - lightPosition[k];
						double cosAlpha = lightRay.findCos(intersectedTriangle.getNormal());
						R += lightColour[k].getX() * lightModel.getX() * abs(cosAlpha) * 255 * lightIntesities[k] / pow((lightPosition[k] - intersectionPoint).getLength(), 2);
						G += lightColour[k].getY() * lightModel.getY() * abs(cosAlpha) * 255 * lightIntesities[k] / pow((lightPosition[k] - intersectionPoint).getLength(), 2);
						B += lightColour[k].getZ() * lightModel.getZ() * abs(cosAlpha) * 255 * lightIntesities[k] / pow((lightPosition[k] - intersectionPoint).getLength(), 2);
						
					}
				}
				
				if(R>255) R=255;
				if(G>255) G=255;
				if(B>255) B=255;
				plane.pixels[i][j].R=R;
				plane.pixels[i][j].G=G;
				plane.pixels[i][j].B=B;
				
			}
			else
			{
				plane.pixels[i][j].R=30;
				plane.pixels[i][j].G=30;
				plane.pixels[i][j].B=30;
			}
		}
	}
	workWithBMP::createBMPImage(heightOfScreen, widthOfScreen, plane.pixels);
	clock_t end_time = clock();
	cout << "\nImage rendered! Render time: " <<(end_time - start_time2) / CLOCKS_PER_SEC << " seconds\nTotal time: "<< (double)(end_time - start_time) / CLOCKS_PER_SEC << " seconds." << endl;
}

void takeParameters(string& pathToFile, vector3d& cameraPosition, vector3d& lookAtPoint, vector<vector3d>& lightPosition, int& widthOfScreen, int& heightOfScreen, vector<int>& intens, vector<vector3d>& lightColour, vector3d& lightModel)
{
	string dataFile;
	cout<<"Enter name of file: ";
	cin>>dataFile;
	ifstream inFile(dataFile);
	double temp;
	string info;
	inFile>>info;
	inFile>>pathToFile;
	inFile>>info;
	inFile >> temp;
	cameraPosition.setX(temp);
	inFile >> temp;
	cameraPosition.setY(temp);
	inFile >> temp;
	cameraPosition.setZ(temp);
	inFile>>info;
	inFile >> temp;
	lookAtPoint.setX(temp);
	inFile >> temp;
	lookAtPoint.setY(temp);
	inFile >> temp;
	lookAtPoint.setZ(temp);
	inFile>>info;
	int numberOfLightSources;
	inFile >> numberOfLightSources;
	lightPosition.resize(numberOfLightSources);
	lightColour.resize(numberOfLightSources);
	intens.resize(numberOfLightSources);
	for (int i = 0; i < numberOfLightSources; i++)
	{
		inFile>>info;
		inFile >> temp;
		lightPosition[i].setX(temp);
		inFile >> temp;
		lightPosition[i].setY(temp);
		inFile >> temp;
		lightPosition[i].setZ(temp);
		inFile>>info;
		inFile >> intens[i];
		inFile>>info;
		inFile >> temp;
		lightColour[i].setX(temp / 255);
		inFile >> temp;
		lightColour[i].setY(temp / 255);
		inFile >> temp;
		lightColour[i].setZ(temp / 255);
	}
	inFile>>info;
	inFile >> temp;
	lightModel.setX(temp/255);
	inFile >> temp;
	lightModel.setY(temp/255);
	inFile >> temp;
	lightModel.setZ(temp/255);
	inFile>>info;
	inFile >> widthOfScreen;
	inFile>>info;
	inFile >> heightOfScreen;
	inFile.close();
}