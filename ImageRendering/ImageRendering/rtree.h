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
    void insert(triangle);
    std::vector<Node> ChooseLeaf(Node*, triangle);
    std::vector<Node> DoInsert(Node*, triangle);
    std::vector<Node> LinearSplit(std::vector<triangle>);
    std::vector<Node> LinearSplitNodes(std::vector<Node*> Nodes);
    void AdjustBounds(Node*, triangle);
    Node* MinimalResize(Node*, triangle);
    double getVolume(Node* current);
};

