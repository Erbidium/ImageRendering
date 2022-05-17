#include "graphicalScene.h"
#include "workWithBMP.h"

graphicalScene::graphicalScene():start_time(0), plane(nullptr), start_renderTime(0)
{
	fileReader::takeParameters(pathToObjFile, cameraPosition, lookAtPoint, lightPosition, widthOfScreen, heightOfScreen, lightIntesities, lightColour, lightModel, angle);
	cameraDirection = (lookAtPoint - cameraPosition) / ((lookAtPoint - cameraPosition).getLength());
}

void graphicalScene::buildTree()
{
	start_time= clock();
	vector<triangle> triangles =  fileReader::readObj(pathToObjFile, angle);
	for(int i=0;i<triangles.size();i++)
	{
		treeOfModel.insert(triangles[i]);
	}
	cout<<"\nTree build! Tree building time: "<< (double)(clock() - start_time) / CLOCKS_PER_SEC << " seconds." <<endl;
}

void graphicalScene::renderPixels()
{
	plane=new projectionPlane(widthOfScreen, heightOfScreen, cameraPosition+cameraDirection, cameraDirection, cameraPosition);
	vector<vector<vector3d>> pixelsInWorld=plane->getPixelsCoordinatesInWorld();
	vector3d intersectionPoint;
	vector3d directionOfRay;
	triangle intersectedTriangle;
	start_renderTime = clock();
	std::vector<std::vector<PIXEL>> pixels=plane->getPixelMatrix();
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
				if(R>255) 
					R=255;
				if(G>255) 
					G=255;
				if(B>255) 
					B=255;
				pixels[i][j].R=R;
				pixels[i][j].G=G;
				pixels[i][j].B=B;
			}
			else
			{
				pixels[i][j].R=30;
				pixels[i][j].G=30;
				pixels[i][j].B=30;
			}
		}
	}
	plane->setPixelMatrix(pixels);
}

void graphicalScene::pixelsToFile()const
{
	workWithBMP::createBMPImage(heightOfScreen, widthOfScreen, plane->getPixelMatrix());
	clock_t end_time = clock();
	cout << "\nImage rendered! Render time: " <<(end_time - start_renderTime) / (double)CLOCKS_PER_SEC << " seconds\nTotal time: "<< (double)(end_time - start_time) / CLOCKS_PER_SEC << " seconds." << endl;
}

graphicalScene::~graphicalScene()
{
	delete plane;
}
