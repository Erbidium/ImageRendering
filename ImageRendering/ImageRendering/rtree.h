#pragma once
#include <vector>
#include "triangle.h"
#include "Node.h"

class Rtree
{
    const int maxEntries = 4;
    const int maxNumberOfLeafs = 5;
    Node* root;
	bool findIntersectionInTree(vector3d rayOrigin, vector3d rayVector, vector3d& outIntersectionPoint, Node * current, bool&finished);
public:
    Rtree() { root = new Node; }
	//~Rtree(){ delete root; }
	int count();
	void countTrigs(int &number, Node*current);
    void insert(triangle);
	bool intersectionOfRayAnd3Dmodel(vector3d rayOrigin, vector3d rayVector, vector3d& outIntersectionPoint);
    std::vector<Node*> ChooseLeaf(Node*, triangle);
    std::vector<Node*> DoInsert(Node*, triangle);
    std::vector<Node*> LinearSplit(std::vector<triangle>);
    std::vector<Node*> LinearSplitNodes(std::vector<Node*> Nodes, Node*);
    void AdjustBounds(Node*, triangle);
    void AdjustBoundsRect(Node* current, double x_max, double x_min, double y_max, double y_min, double z_max, double z_min);
    Node* MinimalResize(Node*, triangle);
    Node* MinimalResizeRect(Node* current, double x_max, double y_max, double z_max, double x_min, double y_min, double z_min);
    double getVolume(Node* current);
};

