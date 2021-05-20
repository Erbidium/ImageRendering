#include "Rtree.h"

#include <iostream>

#include "intersectionChecker.h"
using namespace std;

int globalCounter=0;

void Rtree::countTrigs(int &number, Node * current)
{
	if(current!=nullptr)
	{
		if(current->childs.empty())
		{
			number+=current->triangles.size();
			//cout<<"Did sum "<<number<<endl;
		}
		else
		{
			for(int i=0;i<current->childs.size();i++)
			{
				countTrigs(number, current->childs[i]);
			}
		}
	}
}

void Rtree::insert(triangle trig)
{
	vector<Node*> splitNodes = ChooseLeaf(this->root, trig);
	AdjustBounds(root, trig);
	if (splitNodes.size() > 0)
	{
		Node *newRoot=new Node;
		root = newRoot;
		for (int i = 0; i < splitNodes.size(); i++)
		{
			root->childs.push_back(splitNodes[i]);
			AdjustBoundsRect(root, root->childs[i]->x_max, root->childs[i]->x_min, root->childs[i]->y_max, root->childs[i]->y_min, root->childs[i]->z_max, root->childs[i]->z_min);
		}
	}
}

bool Rtree::intersectionOfRayAnd3Dmodel(vector3d rayOrigin, vector3d rayVector, vector3d& outIntersectionPoint)
{
	bool finished=false;
	if(!intersectionChecker::intersectionRayAndBox(rayVector, rayOrigin, root))
		return false;
	else
		return findIntersectionInTree(rayOrigin, rayVector, outIntersectionPoint, root, finished);
}

vector<Node*> Rtree::ChooseLeaf(Node* current, triangle trig)
{
	vector<Node*> SplitNodes;
	if (current->childs.empty())
	{
		SplitNodes = DoInsert(current, trig);
		if (SplitNodes.empty())
		{
			AdjustBounds(current, trig);
		}
		else
			current->triangles.clear();
		return SplitNodes;
	}
	else
	{
		Node * optimalNode=MinimalResize(current, trig);
		SplitNodes = ChooseLeaf(optimalNode, trig);
		AdjustBounds(current, trig);
		if (SplitNodes.size() > 0)
		{
			int indexOfOptimal=0;
			for(int i=0;i<current->childs.size();i++)
			{
				if(current->childs[i]==optimalNode)
				{
					indexOfOptimal=i;
					break;
				}
			}
			//delete current->childs[indexOfOptimal];
			//current->childs[indexOfOptimal]=nullptr;
			current->childs.erase(current->childs.begin()+indexOfOptimal);
			if (current->childs.size() < maxEntries)
			{
				for(int i=0;i<SplitNodes.size();i++)
				{
					current->childs.push_back(SplitNodes[i]);
					AdjustBoundsRect(current, SplitNodes[i]->x_max, SplitNodes[i]->x_min, SplitNodes[i]->y_max, SplitNodes[i]->y_min, SplitNodes[i]->z_max, SplitNodes[i]->z_min);
				}
				SplitNodes.clear();
			}
			else
			{
				SplitNodes = LinearSplitNodes(SplitNodes, current);
			}
		}
		return SplitNodes;
	}
}

vector<Node*> Rtree::DoInsert(Node* current, triangle trig)
{
	vector<Node*> SplitNodes;
	if (current->triangles.size() < maxNumberOfLeafs)
	{
		current->triangles.push_back(trig);
	}
	else
	{
		vector<triangle> trigs = current->triangles;
		trigs.push_back(trig);
		SplitNodes = LinearSplit(trigs);
		current->triangles.clear();
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

void Rtree::AdjustBoundsRect(Node* current, double x_max, double x_min, double y_max, double y_min, double z_max, double z_min)
{
	if (current->x_max < x_max)
	{
		current->x_max = x_max;
	}
	if (current->y_max < y_max)
	{
		current->y_max = y_max;
	}
	if (current->z_max < z_max)
	{
		current->z_max = z_max;
	}
	if (current->x_min > x_min)
	{
		current->x_min = x_min;
	}
	if (current->y_min > y_min)
	{
		current->y_min = y_min;
	}
	if (current->z_min > y_min)
	{
		current->z_min = y_min;
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

Node* Rtree::MinimalResizeRect(Node* current, double x_max, double y_max, double z_max, double x_min, double y_min, double z_min)
{
	double MinimalDeltaVolume = INT_MAX;
	int indexofChild;
	for (int i = 0; i < current->childs.size(); i++)
	{
		Node* temp = current->childs[i];
		double PreviousVolume = getVolume(temp);
		AdjustBoundsRect(temp, x_max, x_min, y_max, y_min, z_max, z_min);
		double NewVolume = getVolume(temp);
		if (MinimalDeltaVolume > NewVolume - PreviousVolume)
		{
			MinimalDeltaVolume = NewVolume - PreviousVolume;
			indexofChild = i;
		}
	}
	return current->childs[indexofChild];
}

double Rtree::getVolume(Node* current)
{
	return ((current->x_max - current->x_min)*(current->y_max - current->y_min)*(current->z_max - current->z_min));
}

vector<Node*> Rtree::LinearSplit(vector<triangle> trigs)
{
	int indexOfLeaf1 = 0, indexOfLeaf2 = 1;
	double maxDistance = (trigs[indexOfLeaf1].getCenter() - trigs[indexOfLeaf2].getCenter()).getLength();
	for (int i = 0; i < trigs.size(); i++)
	{
		for (int j = i + 1; j < trigs.size(); j++)
		{
			double tempDistance = (trigs[i].getCenter() - trigs[j].getCenter()).getLength();
			if (tempDistance > maxDistance)
			{
				maxDistance = tempDistance;
				indexOfLeaf1 = i;
				indexOfLeaf2 = j;
			}
		}
	}
	Node* leaf1 = new Node;
	leaf1->triangles.push_back(trigs[indexOfLeaf1]);
	Node* leaf2 = new Node;
	leaf2->triangles.push_back(trigs[indexOfLeaf2]);
	AdjustBounds(leaf1, trigs[indexOfLeaf1]);
	AdjustBounds(leaf2, trigs[indexOfLeaf2]);
	vector3d leaf1Center = leaf1->triangles[0].getCenter();
	vector3d leaf2Center = leaf2->triangles[0].getCenter();
	trigs.erase(trigs.begin() + indexOfLeaf1);
	if (indexOfLeaf1 > indexOfLeaf2)
	{
		trigs.erase(trigs.begin() + indexOfLeaf2);
	}
	else
	{
		trigs.erase(trigs.begin() + indexOfLeaf2 - 1);
	}
	for (int i = 0; i < trigs.size(); i++)
	{
		triangle currentTriangle = trigs[i];
		vector3d currentCenter = currentTriangle.getCenter();
		double distanceToFirstLeaf = (currentCenter - leaf1Center).getLength();
		double distanceToSecondLeaf = (currentCenter - leaf2Center).getLength();
		if (distanceToFirstLeaf > distanceToSecondLeaf)
		{
			leaf1->triangles.push_back(currentTriangle);
			AdjustBounds(leaf1, currentTriangle);
		}
		else
		{
			leaf2->triangles.push_back(currentTriangle);
			AdjustBounds(leaf2, currentTriangle);
		}
	}
	vector<Node*> SplitNodes;
	SplitNodes.push_back(leaf1);
	SplitNodes.push_back(leaf2);
	return SplitNodes;
}

bool Rtree::findIntersectionInTree(vector3d rayOrigin, vector3d rayVector, vector3d& outIntersectionPoint, Node * current, bool&finished)
{
	if(current->childs.empty())
	{
		bool wasIntersection=false;
		for(int i=0;(i<current->triangles.size())&&(!wasIntersection);i++)
		{
			wasIntersection=intersectionChecker::rayIntersectsTriangle(rayOrigin, rayVector, &(current->triangles[i]), outIntersectionPoint);
			if(wasIntersection==true)
			{
				finished=true;
				break;
			}
		}
		return wasIntersection;
	}
	else
	{
		for(int i=0;(i<current->childs.size())&&(!finished);i++)
		{
			bool wasIntersectionWithRect=intersectionChecker::intersectionRayAndBox(rayVector, rayOrigin, current->childs[i]);
			if(wasIntersectionWithRect)
				findIntersectionInTree(rayOrigin, rayVector, outIntersectionPoint, current->childs[i], finished);
		}
		
		return finished;
	}
}

int Rtree::count()
{
	int number=0;
	countTrigs(number, root);
	return number;
}

vector<Node*> Rtree::LinearSplitNodes(vector<Node*> Spleet, Node* current)
{
	vector<Node*> SplitNodes(2);
	SplitNodes[0]=new Node;
	SplitNodes[1]=new Node;
	for (int i = 0; i < Spleet.size(); i++)
	{
		SplitNodes[0]->childs.push_back(Spleet[i]);
		AdjustBoundsRect(SplitNodes[0], SplitNodes[0]->childs[i]->x_max, SplitNodes[0]->childs[i]->x_min, SplitNodes[0]->childs[i]->y_max, SplitNodes[0]->childs[i]->y_min, SplitNodes[0]->childs[i]->z_max, SplitNodes[0]->childs[i]->z_min);
	}
	for (int i = 0; i < current->childs.size(); i++)
	{
		SplitNodes[1]->childs.push_back(current->childs[i]);
		AdjustBoundsRect(SplitNodes[1], current->childs[i]->x_max, current->childs[i]->x_min, current->childs[i]->y_max, current->childs[i]->y_min, current->childs[i]->z_max, current->childs[i]->z_min);
	}
	current->childs.clear();//
	current->triangles.clear();
	return SplitNodes;
	////Rework LinearSplitNodes and optimally divide Nodes of rtree
}
