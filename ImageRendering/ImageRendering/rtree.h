#pragma once
#include <vector>
#include "triangle.h"
#include "node.h"

class rtree
{
    const int maxEntries = 4;
    const int maxNumberOfLeafs = 5;
    node* root;
	bool findIntersectionInTree(vector3d rayOrigin, vector3d rayVector, vector3d &outIntersectionPoint, node *current, bool &wasIntersection, triangle &intersectedTriangle);
public:
    rtree() { root = new node; }
	~rtree(){ delete root; }
    void insert(triangle);
	bool intersectionOfRayAnd3Dmodel(vector3d rayOrigin, vector3d rayVector, vector3d &outIntersectionPoint, triangle &intersectedTriangle);
    bool findIntersectionLigthInTree(vector3d lightPoint, vector3d lightVector, node *current, vector3d &intersectionPoint, triangle &intersectedTriangle);
    std::vector<node*> ChooseLeaf(node*, triangle);
    std::vector<node*> DoInsert(node*, triangle);
    std::vector<node*> LinearSplit(std::vector<triangle>);
    std::vector<node*> LinearSplitNodes(std::vector<node*> Spleet, node* current);
    void AdjustBounds(node*, triangle);
    void AdjustBoundsRect(node* current, double x_max, double x_min, double y_max, double y_min, double z_max, double z_min);
    node* MinimalResize(node*, triangle);
    double getVolume(node* current);
    node* getRoot();
};

