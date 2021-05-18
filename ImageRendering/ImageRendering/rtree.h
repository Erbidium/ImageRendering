#pragma once
#include <vector>
#include "triangle.h"

class rtree
{
    const int maxEntries=4;
    const int minEntries=2;
    const int maxNumberOfLeafs=5;
    std::vector<rtree*> childs;
    std::vector<triangle> triangles;
    rtree(){}
    rtree(triangle newTriangle)
    {
        triangles.push_back(newTriangle);
    }
    rtree(std::vector<rtree*> childs)
    {
        triangles.push_back(newTriangle);
    }
public:
    void insert(triangle newTriangle)
    {
        std::vector<rtree*>splitNodes;
        rtree * leafNode=new rtree(newTriangle);
        chooseLeaf(splitNodes, leafNode);
        adjustBounds();
        if(splitNodes.size()>0)
        {
            rtree * newRoot;
        }
    }
    void chooseLeaf(std::vector<rtree*>&splitNodes, rtree * currentTreeNode)
    {
        
    }
    void adjustBounds()
    {

    }
}
};

