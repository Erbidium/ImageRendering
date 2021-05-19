#pragma once
#include <vector>
#include "triangle.h"
#include "Node.h"

class Rtree
{
    const int maxEntries = 4;
    const int minEntries = 2;
    const int maxNumberOfLeafs = 5;
    Node* root;
public:
    Rtree() { root = new Node; }
	~Rtree(){ delete root; }
    void insert(triangle);
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

