#pragma once
#include <vector>
#include "triangle.h"
#include "vector3d.h"
#include "cmath"

class rtree
{
    const int maxEntries=4;
    const int maxNumberOfLeafs=10;
    vector3d maxBorders;
    vector3d minBorders;
    std::vector<rtree*> childs;
    std::vector<triangle> triangles;
public:
	rtree(){}
    rtree(triangle newTriangle)
    {
        triangles.push_back(newTriangle);
    }
    std::vector<rtree*> getChildren(){return childs;}
	void addTriangle(triangle newTriangle)
	{
		triangles.push_back(newTriangle);
	}
	void clearTringles()
	{
		triangles.clear();
	}
    std::vector<triangle> getTriangles()
    {
        return triangles;
    }
	void linearSplit(triangle newTriangle)
    {
	    std::vector<triangle> allTriangles=triangles;
		allTriangles.push_back(newTriangle);
    	int indexOfLeaf1=0, indexOfLeaf2=1;
    	double maxDistance=(allTriangles[indexOfLeaf1].getCenter()-allTriangles[indexOfLeaf2].getCenter()).getLength();
    	for(int i=0;i<allTriangles.size();i++)
    	{
    		for(int j=i+1;j<allTriangles.size();j++)
    		{
				double tempDistance=(allTriangles[i].getCenter()-allTriangles[j].getCenter()).getLength();
    			if(tempDistance>maxDistance)
    			{
    				maxDistance=tempDistance;
    				indexOfLeaf1=i;
    				indexOfLeaf2=j;
    			}
    		}
    	}
    	rtree* leaf1=new rtree(allTriangles[indexOfLeaf1]);
    	rtree* leaf2=new rtree(allTriangles[indexOfLeaf2]);
		leaf1->adjustBounds(allTriangles[indexOfLeaf1]);
		leaf1->adjustBounds(allTriangles[indexOfLeaf2]);
    	vector3d leaf1Center=leaf1->getTriangles()[0].getCenter();
    	vector3d leaf2Center=leaf2->getTriangles()[0].getCenter();
    	allTriangles.erase(allTriangles.begin()+indexOfLeaf1);
		if(indexOfLeaf1>indexOfLeaf2)
    		allTriangles.erase(allTriangles.begin()+indexOfLeaf2);
        else
            allTriangles.erase(allTriangles.begin()+indexOfLeaf2-1);
    	for(int i=0;i<allTriangles.size();i++)
    	{
    		triangle currentTriangle=allTriangles[i];
    		vector3d currentCenter=currentTriangle.getCenter();
    		double distanceToFirstLeaf=(currentCenter-leaf1Center).getLength();
    		double distanceToSecondLeaf=(currentCenter-leaf2Center).getLength();
    		if(distanceToFirstLeaf>distanceToSecondLeaf)
            {
            	leaf1->addTriangle(currentTriangle);
            	leaf1->adjustBounds(currentTriangle);
            }
            else
            {
	            leaf2->addTriangle(currentTriangle);
            	leaf2->adjustBounds(currentTriangle);
            }
    	}
		childs.push_back(leaf1);
		childs.push_back(leaf2);
		clearTringles();
    }
    void doInsert(triangle newTriangle)
    {
        if(triangles.size()<maxNumberOfLeafs)
            triangles.push_back(newTriangle);
        else
        {
            linearSplit(newTriangle);
        }
    }
	double getVolume()
    {
	    return (maxBorders.getX()-minBorders.getX())*(maxBorders.getY()-minBorders.getY())*(maxBorders.getZ()-minBorders.getZ());
    }
    void adjustBounds(triangle newTriangle)
    {
	    vector3d maxInTriangle = newTriangle.getVertex1();
        vector3d minInTriangle = newTriangle.getVertex1();
        if(maxInTriangle.getX()<std::max(newTriangle.getVertex2().getX(), newTriangle.getVertex3().getX()))
            maxInTriangle.setX(std::max(newTriangle.getVertex2().getX(), newTriangle.getVertex3().getX()));
        if(maxInTriangle.getY()<std::max(newTriangle.getVertex2().getY(), newTriangle.getVertex3().getY()))
            maxInTriangle.setY(std::max(newTriangle.getVertex2().getY(), newTriangle.getVertex3().getY()));
        if(maxInTriangle.getZ()<std::max(newTriangle.getVertex2().getZ(), newTriangle.getVertex3().getZ()))
            maxInTriangle.setZ(std::max(newTriangle.getVertex2().getZ(), newTriangle.getVertex3().getZ()));
        if(maxBorders.getX()<maxInTriangle.getX())
            maxBorders.setX(maxInTriangle.getX());
        if(maxBorders.getY()<maxInTriangle.getY())
            maxBorders.setY(maxInTriangle.getY());
        if(maxBorders.getZ()<maxInTriangle.getZ())
            maxBorders.setZ(maxInTriangle.getZ());

        if(minInTriangle.getX()>std::max(newTriangle.getVertex2().getX(), newTriangle.getVertex3().getX()))
            minInTriangle.setX(std::max(newTriangle.getVertex2().getX(), newTriangle.getVertex3().getX()));
        if(minInTriangle.getY()>std::max(newTriangle.getVertex2().getY(), newTriangle.getVertex3().getY()))
            minInTriangle.setY(std::max(newTriangle.getVertex2().getY(), newTriangle.getVertex3().getY()));
        if(maxInTriangle.getZ()>std::max(newTriangle.getVertex2().getZ(), newTriangle.getVertex3().getZ()))
            minInTriangle.setZ(std::max(newTriangle.getVertex2().getZ(), newTriangle.getVertex3().getZ()));
        if(minBorders.getX()<minInTriangle.getX())
            minBorders.setX(minInTriangle.getX());
        if(minBorders.getY()<minInTriangle.getY())
            minBorders.setY(minInTriangle.getY());
        if(minBorders.getZ()<minInTriangle.getZ())
            minBorders.setZ(minInTriangle.getZ());
    }
    void insert(triangle newTriangle)
    {
        chooseLeaf(newTriangle);
        adjustBounds(newTriangle);
    }
    void chooseLeaf(triangle newTriangle)
    {
        if(childs.empty())
        {
            doInsert(newTriangle);
            adjustBounds(newTriangle);
        }
        else
        {
        	triangle currentTriangle=newTriangle;
            rtree temp=*childs[0];
        	int indexOfChild=0;
        	double previousVolume=temp.getVolume();
        	temp.adjustBounds(currentTriangle);
        	double minimalDeltaVolume=temp.getVolume()-previousVolume;
        	for(int i=1;i<childs.size();i++)
        	{
        		rtree temp=*(childs[i]);
        		previousVolume=temp.getVolume();
        		temp.adjustBounds(currentTriangle);
        		if(minimalDeltaVolume>temp.getVolume()-previousVolume)
        		{
        			minimalDeltaVolume=temp.getVolume()-previousVolume;
        			indexOfChild=i;
        		}
        	}
            rtree * optimalNode=childs[indexOfChild];
            optimalNode->chooseLeaf(currentTriangle);
            adjustBounds(currentTriangle);
		}
    }
};