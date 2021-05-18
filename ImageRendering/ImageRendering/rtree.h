#pragma once
#include <vector>
#include "triangle.h"
#include "vector3d.h"
#include "cmath"

class rtree
{
    const int maxEntries=4;
    const int minEntries=2;
    const int maxNumberOfLeafs=5;
    vector3d maxBorders;
    vector3d minBorders;
    std::vector<rtree*> childs;
    std::vector<triangle> triangles;
    rtree * root;
    rtree():root(nullptr){}
    rtree(triangle newTriangle):root(nullptr)
    {
        triangles.push_back(newTriangle);
    }
    std::vector<rtree*> getChildren(){return childs;}
public:
    std::vector<triangle> getTriangles()
    {
        return triangles;
    }
    void doInsert(std::vector<rtree*>&splitNodes, rtree * leaf)
    {
        if(triangles.size()<maxNumberOfLeafs)
            triangles.push_back(leaf->getTriangles()[0]);
        else
        {
            linearSplit(splitNodes);
        }
    }
	double getVolume()
    {
	    return (maxBorders.getX()-minBorders.getX())*(maxBorders.getY()-minBorders.getY())*(maxBorders.getZ()-minBorders.getZ());
    }
    void adjustBounds(triangle newTriangle)
    {
        vector3d maxInTriangle, minInTriangle;
        maxInTriangle=newTriangle.getVertex1();
        minInTriangle=newTriangle.getVertex1();
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
    void linearSplit(std::vector<rtree*>&splitNodes)
    {

    }
    void insert(triangle newTriangle)
    {
        std::vector<rtree*>splitNodes;
        rtree * leafNode=new rtree(newTriangle);
        chooseLeaf(splitNodes, leafNode);
        adjustBounds(newTriangle);
        if(splitNodes.size()>0)
        {
            childs.clear();
            for(int i=0;i<splitNodes.size();i++)
            {
                childs.push_back(splitNodes[i]);
            }
        }
    }
    void chooseLeaf(std::vector<rtree*>&splitNodes, rtree * currentTreeNode)
    {
        if(currentTreeNode->getChildren().size()==0)
        {
            doInsert(splitNodes, currentTreeNode);
            adjustBounds(currentTreeNode->getTriangles()[0]);
        }
        else
        {
        	triangle currentTriangle=currentTreeNode->getTriangles()[0];
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
        		if(minimalDeltaVolume<temp.getVolume()-previousVolume)
        		{
        			minimalDeltaVolume=temp.getVolume()-previousVolume;
        			indexOfChild=i;
        		}
        	}
            rtree * optimalNode=childs[indexOfChild];
            chooseLeaf(splitNodes, optimalNode);
            adjustBounds(currentTriangle);
            if(!splitNodes.empty())
            {
                if(childs.size()<maxEntries)
                {
	                childs.push_back(splitNodes[0]);
                	splitNodes.clear();
                }
            }
		}
    }
};

