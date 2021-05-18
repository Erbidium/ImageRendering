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
    rtree * root;
    rtree():root(nullptr){}
    rtree(triangle newTriangle):root(nullptr)
    {
        triangles.push_back(newTriangle);
    }
    std::vector<rtree*> getChildren(){return childs;}
public:
    void doInsert(std::vector<rtree*>&splitNodes)
    {

    }
    void adjustBounds()
    {

    }
    void insert(triangle newTriangle)
    {
        std::vector<rtree*>splitNodes;
        rtree * leafNode=new rtree(newTriangle);
        chooseLeaf(splitNodes, leafNode);
        adjustBounds();
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
            doInsert(splitNodes);
            adjustBounds();
        }
        else
        {
            ///choose minimal growth of volume
            rtree * optimalNode;/////
            chooseLeaf(splitNodes, optimalNode);
            adjustBounds();
            if(splitNodes.size()>0)
            {
                if(childs.size()<maxEntries)
                    ////unwritten code
            }
    }
    
    
}
};

