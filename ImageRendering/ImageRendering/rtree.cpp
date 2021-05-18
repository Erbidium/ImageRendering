#include "Rtree.h"
using namespace std;

void Rtree::insert(triangle trig)
{
	vector<Node> splitNodes = ChooseLeaf(root, trig);
	if (splitNodes.size() > 0)
	{
		Node newRoot;
		root = &newRoot;
		for (int i = 0; i < splitNodes.size(); i++)
		{
			root->childs.push_back(&splitNodes[i]);
		}
	}
	AdjustBounds(root, trig);
}

vector<Node> Rtree::ChooseLeaf(Node* current, triangle trig)
{
	vector<Node> SplitNodes;
	if (current->childs.size() == 0)
	{
		SplitNodes = DoInsert(current, trig);
		if (SplitNodes.size() == 0)
		{
			AdjustBounds(current, trig);
		}
		return SplitNodes;
	}
	else
	{
		SplitNodes = ChooseLeaf(MinimalResize(current, trig), trig);
		AdjustBounds(current, trig);
		if (SplitNodes.size() > 0)
		{
			if (current->childs.size() < maxEntries)
			{
				Node* newNode;
				newNode->triangles.push_back(trig);
				AdjustBounds(newNode, trig);
				current->childs.push_back(newNode);
				SplitNodes.clear();
			}
			else
			{
				vector<Node*> Nodes = current->childs;
				for (int i = 0; i < SplitNodes.size(); i++)
				{
					for (int j = 0; j < SplitNodes[i].childs.size(); i++)
					{
						Nodes.push_back(SplitNodes[i].childs[j]);
					}
				}
				SplitNodes = LinearSplitNodes(Nodes);
			}
		}
	}
}

vector<Node> Rtree::DoInsert(Node* current, triangle trig)
{
	vector<Node> SplitNodes;
	if (current->triangles.size() < maxNumberOfLeafs)
	{
		current->triangles.push_back(trig);
	}
	else
	{
		vector<triangle> trigs = current->triangles;
		trigs.push_back(trig);
		SplitNodes = LinearSplit(trigs);
	}
	return SplitNodes;
}

void Rtree::AdjustBounds(Node* current, triangle trig)
{
	if (current->x_max < trig.getX_max())
	{
		current->x_max = trig.getX_max();
	}
	if (current->y_max < trig.getY_max())
	{
		current->y_max = trig.getY_max();
	}
	if (current->z_max < trig.getZ_max())
	{
		current->z_max = trig.getZ_max();
	}
	if (current->x_min > trig.getX_min())
	{
		current->x_min = trig.getX_min();
	}
	if (current->y_min > trig.getY_min())
	{
		current->y_min = trig.getY_min();
	}
	if (current->z_min > trig.getZ_min())
	{
		current->z_min = trig.getZ_min();
	}
}

Node* Rtree::MinimalResize(Node* current, triangle trig)
{
	double MinimalDeltaVolume = INT_MAX;
	int indexOfChild;
	for (int i = 0; i < current->childs.size(); i++)
	{
		Node* temp = current->childs[i];
		double PreviousVolume = getVolume(temp);
		AdjustBounds(temp, trig);
		double NewVolume = getVolume(temp);
		if (MinimalDeltaVolume > NewVolume - PreviousVolume)
		{
			MinimalDeltaVolume = NewVolume - PreviousVolume;
			indexOfChild = i;
		}
	}
	return current->childs[indexOfChild];
}

double Rtree::getVolume(Node* current)
{
	return ((current->x_max - current->x_min)*(current->y_max - current->y_min)*(current->z_max - current->z_min));
}

vector<Node> Rtree::LinearSplit(vector<triangle> trigs)
{

}

vector<Node> Rtree::LinearSplitNodes(vector<Node*> Nodes)
{

}