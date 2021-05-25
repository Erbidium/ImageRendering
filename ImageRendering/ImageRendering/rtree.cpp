#include "Rtree.h"

#include <iostream>

#include "intersectionChecker.h"
using namespace std;

int globalCounter=0;

void Rtree::countTrigs(int &number, Node*current, std::vector<triangle>&trigs)
{
	if(current!=nullptr)
	{
		if(current->childs.empty())
		{
			number+=current->triangles.size();
			for(int i=0;i<current->triangles.size();i++)
			{
				trigs.push_back(current->triangles[i]);
			}
			//cout<<"Did sum "<<number<<endl;
		}
		else
		{
			for(int i=0;i<current->childs.size();i++)
			{
				countTrigs(number, current->childs[i], trigs);
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

bool Rtree::intersectionOfRayAnd3Dmodel(vector3d rayOrigin, vector3d rayVector, vector3d& outIntersectionPoint, triangle& intersectedTriangle)
{
	bool wasInetersection=false;
	//return intersectionChecker::intersectionRayAndBox(rayVector, rayOrigin, root);
	if(!intersectionChecker::intersectionRayAndBox(rayVector, rayOrigin, root))
		return false;
	else
		return findIntersectionInTree(rayOrigin, rayVector, outIntersectionPoint, root, wasInetersection, intersectedTriangle);
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
		{
			//current->triangles.clear();
			//current->setDefaultRect();
		}
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
			current->childs.erase(current->childs.begin()+indexOfOptimal);////
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
		//AdjustBounds(current, trig);
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
	double eps=0.0000001;
	if (current->x_max < trig.getX_max())
	{
		current->x_max = trig.getX_max()+eps;
	}
	if (current->y_max < trig.getY_max())
	{
		current->y_max = trig.getY_max()+eps;
	}
	if (current->z_max < trig.getZ_max())
	{
		current->z_max = trig.getZ_max()+eps;
	}
	if (current->x_min > trig.getX_min())
	{
		current->x_min = trig.getX_min()-eps;
	}
	if (current->y_min > trig.getY_min())
	{
		current->y_min = trig.getY_min()-eps;
	}
	if (current->z_min > trig.getZ_min())
	{
		current->z_min = trig.getZ_min()-eps;
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
	if (current->z_min > z_min)
	{
		current->z_min = z_min;
	}
}

Node* Rtree::MinimalResize(Node* current, triangle trig)
{
	double MinimalDeltaVolume = INT_MAX;
	int indexOfChild;
	for (int i = 0; i < current->childs.size(); i++)
	{
		Node *temp = new Node;
		*temp=current->childs[i];
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
		Node *temp=new Node;
		*temp=current->childs[i];
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
		double oldVolume1 = getVolume(leaf1);
		double oldVolume2 = getVolume(leaf2);
		triangle currentTriangle = trigs[i];
		Node *tempLeaf1 = new Node;
		*tempLeaf1=leaf1;
		Node *tempLeaf2 = new Node;
		*tempLeaf2=leaf2;
		AdjustBounds(tempLeaf1, trigs[i]);
		double VoulumeOfFirst = getVolume(tempLeaf1);
		AdjustBounds(tempLeaf2, trigs[i]);
		double VolumeOfSecondLeaf = getVolume(tempLeaf2);
		if (VoulumeOfFirst - oldVolume1 > VolumeOfSecondLeaf - oldVolume2)
		{
			leaf2->triangles.push_back(currentTriangle);
			AdjustBounds(leaf2, currentTriangle);
		}
		else
		{
			leaf1->triangles.push_back(currentTriangle);
			AdjustBounds(leaf1, currentTriangle);
		}
	}
	vector<Node*> SplitNodes;
	SplitNodes.push_back(leaf1);
	SplitNodes.push_back(leaf2);
	return SplitNodes;
}

bool Rtree::findIntersectionInTree(vector3d rayOrigin, vector3d rayVector, vector3d& outIntersectionPoint, Node * current, bool&wasIntersection, triangle& intersectedTriangle)
{
	if(current->childs.empty())
	{
		for(int i=0;i<current->triangles.size();i++)
		{
			vector3d tempIntersectionPoint;
			if(intersectionChecker::rayIntersectsTriangle(rayOrigin, rayVector, &(current->triangles[i]), tempIntersectionPoint))
			{
				if (wasIntersection)
				{
					double distanceOld = (rayOrigin - outIntersectionPoint).getLength();
					double distanceNew = (rayOrigin - tempIntersectionPoint).getLength();
					if (distanceNew < distanceOld)
					{
						intersectedTriangle = current->triangles[i];
						outIntersectionPoint = tempIntersectionPoint;
					}
				}
				else
				{
					intersectedTriangle = current->triangles[i];
					outIntersectionPoint = tempIntersectionPoint;
					wasIntersection = true;
				}
			}
		}
		return wasIntersection;
	}
	else
	{
		for(int i=0;i<current->childs.size();i++)
		{
			bool wasIntersectionWithRect=intersectionChecker::intersectionRayAndBox(rayVector, rayOrigin, current->childs[i]);
			if(wasIntersectionWithRect)
				findIntersectionInTree(rayOrigin, rayVector, outIntersectionPoint, current->childs[i], wasIntersection, intersectedTriangle);
		}
		
		return wasIntersection;
	}
}

int Rtree::count(vector<triangle>&trigs)
{
	int number=0;
	countTrigs(number, root, trigs);
	return number;
}

/*
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
	int i=current->childs.size()-2;
	SplitNodes[0]->childs.push_back(current->childs[i]);
		AdjustBoundsRect(SplitNodes[0], SplitNodes[0]->childs[i]->x_max, SplitNodes[0]->childs[i]->x_min, SplitNodes[0]->childs[i]->y_max, SplitNodes[0]->childs[i]->y_min, SplitNodes[0]->childs[i]->z_max, SplitNodes[0]->childs[i]->z_min);
	i++;
	SplitNodes[0]->childs.push_back(current->childs[i]);
		AdjustBoundsRect(SplitNodes[0], SplitNodes[0]->childs[i]->x_max, SplitNodes[0]->childs[i]->x_min, SplitNodes[0]->childs[i]->y_max, SplitNodes[0]->childs[i]->y_min, SplitNodes[0]->childs[i]->z_max, SplitNodes[0]->childs[i]->z_min);
	for (int i = 0; i < current->childs.size()-2; i++)
	{
		SplitNodes[1]->childs.push_back(current->childs[i]);
		AdjustBoundsRect(SplitNodes[1], current->childs[i]->x_max, current->childs[i]->x_min, current->childs[i]->y_max, current->childs[i]->y_min, current->childs[i]->z_max, current->childs[i]->z_min);
	}
	current->childs.clear();//
	current->triangles.clear();
	return SplitNodes;
	////Rework LinearSplitNodes and optimally divide Nodes of rtree
}
*/

vector<Node*> Rtree::LinearSplitNodes(vector<Node*> Spleet, Node* current)
{
	vector<Node*> SplitNodes(2);
	SplitNodes[0] = new Node;
	SplitNodes[1] = new Node;
	vector<Node*> nodesToSplit = Spleet;
	nodesToSplit.insert(nodesToSplit.end(), current->childs.begin(), current->childs.end());
	int indexOfChild1 = 0, indexOfChild2 = 1;
	double maxDistance = (nodesToSplit[indexOfChild1]->getBoxCenter() - nodesToSplit[indexOfChild2]->getBoxCenter()).getLength();
	for (int i = 0; i < nodesToSplit.size(); i++)
	{
		for (int j = i + 1; j < nodesToSplit.size(); j++)
		{
			double tempDistance = (nodesToSplit[i]->getBoxCenter() - nodesToSplit[j]->getBoxCenter()).getLength();
			if (tempDistance > maxDistance)
			{
				maxDistance = tempDistance;
				indexOfChild1 = i;
				indexOfChild2 = j;
			}
		}
	}
	Node* Child1 = new Node;
	Child1->childs.push_back(nodesToSplit[indexOfChild1]);
	AdjustBoundsRect(Child1, nodesToSplit[indexOfChild1]->x_max, nodesToSplit[indexOfChild1]->x_min, nodesToSplit[indexOfChild1]->y_max, nodesToSplit[indexOfChild1]->y_min, nodesToSplit[indexOfChild1]->z_max, nodesToSplit[indexOfChild1]->z_min);
	Node* Child2 = new Node;
	Child2->childs.push_back(nodesToSplit[indexOfChild2]);
	AdjustBoundsRect(Child2, nodesToSplit[indexOfChild2]->x_max, nodesToSplit[indexOfChild2]->x_min, nodesToSplit[indexOfChild2]->y_max, nodesToSplit[indexOfChild2]->y_min, nodesToSplit[indexOfChild2]->z_max, nodesToSplit[indexOfChild2]->z_min);
	nodesToSplit.erase(nodesToSplit.begin() + indexOfChild1);
	if (indexOfChild1 > indexOfChild2)
	{
		nodesToSplit.erase(nodesToSplit.begin() + indexOfChild2);
	}
	else
	{
		nodesToSplit.erase(nodesToSplit.begin() + indexOfChild2 - 1);
	}
	for (int i = 0; i < nodesToSplit.size(); i++)
	{
		Node *tempChild1 = new Node;
		*tempChild1=Child1;
		Node *tempChild2 = new Node;
		*tempChild2=Child2;
		double OldVolume1 = getVolume(Child1);
		double OldVolume2 = getVolume(Child2);
		AdjustBoundsRect(tempChild1, nodesToSplit[i]->x_max, nodesToSplit[i]->x_min, nodesToSplit[i]->y_max, nodesToSplit[i]->y_min, nodesToSplit[i]->z_max, nodesToSplit[i]->z_min);
		double VoulumeOfFirst = getVolume(tempChild1);
		AdjustBoundsRect(tempChild2, nodesToSplit[i]->x_max, nodesToSplit[i]->x_min, nodesToSplit[i]->y_max, nodesToSplit[i]->y_min, nodesToSplit[i]->z_max, nodesToSplit[i]->z_min);
		double VolumeOfSecondChild = getVolume(tempChild2);
		if (VoulumeOfFirst - OldVolume1 > VolumeOfSecondChild - OldVolume2)
		{
			Child2->childs.push_back(nodesToSplit[i]);
			AdjustBoundsRect(Child2, nodesToSplit[i]->x_max, nodesToSplit[i]->x_min, nodesToSplit[i]->y_max, nodesToSplit[i]->y_min, nodesToSplit[i]->z_max, nodesToSplit[i]->z_min);
		}
		else
		{
			Child1->childs.push_back(nodesToSplit[i]);
			AdjustBoundsRect(Child1, nodesToSplit[i]->x_max, nodesToSplit[i]->x_min, nodesToSplit[i]->y_max, nodesToSplit[i]->y_min, nodesToSplit[i]->z_max, nodesToSplit[i]->z_min);
		}
	}
	SplitNodes[0] = Child1;
	SplitNodes[1] = Child2;
	return SplitNodes;
}
