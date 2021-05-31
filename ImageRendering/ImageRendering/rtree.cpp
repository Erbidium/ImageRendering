#include "rtree.h"
#include <iostream>
#include "intersectionChecker.h"
using namespace std;

void rtree::insert(triangle trig)
{
	vector<node*> splitNodes = ChooseLeaf(this->root, trig);
	AdjustBounds(root, trig);
	if (splitNodes.size() > 0)
	{
		node *newRoot=new node;
		root = newRoot;
		for (int i = 0; i < splitNodes.size(); i++)
		{
			root->childs.push_back(splitNodes[i]);
			AdjustBoundsRect(root, root->childs[i]->x_max, root->childs[i]->x_min, root->childs[i]->y_max, root->childs[i]->y_min, root->childs[i]->z_max, root->childs[i]->z_min);
		}
	}
}

vector<node*> rtree::ChooseLeaf(node* current, triangle trig)
{
	vector<node*> SplitNodes;
	if (current->childs.empty())
	{
		SplitNodes = DoInsert(current, trig);
		if (SplitNodes.empty())
		{
			AdjustBounds(current, trig);
		}
		return SplitNodes;
	}
	else
	{
		node * optimalNode=MinimalResize(current, trig);
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

vector<node*> rtree::DoInsert(node* current, triangle trig)
{
	vector<node*> SplitNodes;
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

void rtree::AdjustBounds(node* current, triangle trig)
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

void rtree::AdjustBoundsRect(node* current, double x_max, double x_min, double y_max, double y_min, double z_max, double z_min)
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

node* rtree::MinimalResize(node* current, triangle trig)
{
	double MinimalDeltaVolume = INT_MAX;
	int indexOfChild;
	for (int i = 0; i < current->childs.size(); i++)
	{
		node *temp = new node;
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

vector<node*> rtree::LinearSplit(vector<triangle> trigs)
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
	node* leaf1 = new node;
	leaf1->triangles.push_back(trigs[indexOfLeaf1]);
	node* leaf2 = new node;
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
	double oldVolume1;
	double oldVolume2;
	triangle currentTriangle;
	node* tempLeaf1 = new node;
	node* tempLeaf2 = new node;
	for (int i = 0; i < trigs.size(); i++)
	{
		oldVolume1 = getVolume(leaf1);
		oldVolume2 = getVolume(leaf2);
		currentTriangle = trigs[i];
		*tempLeaf1=leaf1;
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
	vector<node*> SplitNodes;
	SplitNodes.push_back(leaf1);
	SplitNodes.push_back(leaf2);
	return SplitNodes;
}

vector<node*> rtree::LinearSplitNodes(vector<node*> Spleet, node* current)
{
	vector<node*> SplitNodes(2);
	SplitNodes[0] = new node;
	SplitNodes[1] = new node;
	vector<node*> nodesToSplit = Spleet;
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
	node* Child1 = new node;
	Child1->childs.push_back(nodesToSplit[indexOfChild1]);
	AdjustBoundsRect(Child1, nodesToSplit[indexOfChild1]->x_max, nodesToSplit[indexOfChild1]->x_min, nodesToSplit[indexOfChild1]->y_max, nodesToSplit[indexOfChild1]->y_min, nodesToSplit[indexOfChild1]->z_max, nodesToSplit[indexOfChild1]->z_min);
	node* Child2 = new node;
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
	node* tempChild1 = new node;
	node* tempChild2 = new node;
	double OldVolume1;
	double OldVolume2;
	double VoulumeOfFirst;
	double VolumeOfSecondChild;
	for (int i = 0; i < nodesToSplit.size(); i++)
	{
		*tempChild1=Child1;
		*tempChild2=Child2;
		OldVolume1 = getVolume(Child1);
		OldVolume2 = getVolume(Child2);
		AdjustBoundsRect(tempChild1, nodesToSplit[i]->x_max, nodesToSplit[i]->x_min, nodesToSplit[i]->y_max, nodesToSplit[i]->y_min, nodesToSplit[i]->z_max, nodesToSplit[i]->z_min);
		VoulumeOfFirst = getVolume(tempChild1);
		AdjustBoundsRect(tempChild2, nodesToSplit[i]->x_max, nodesToSplit[i]->x_min, nodesToSplit[i]->y_max, nodesToSplit[i]->y_min, nodesToSplit[i]->z_max, nodesToSplit[i]->z_min);
		VolumeOfSecondChild = getVolume(tempChild2);
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

bool rtree::intersectionOfRayAnd3Dmodel(vector3d rayOrigin, vector3d rayVector, vector3d& outIntersectionPoint, triangle& intersectedTriangle)
{
	if (!intersectionChecker::intersectionRayAndBox(rayVector, rayOrigin, root))
	{
		return false;
	}
	else
	{
		bool wasInetersection = false;
		return findIntersectionInTree(rayOrigin, rayVector, outIntersectionPoint, root, wasInetersection, intersectedTriangle);
	}
}

bool rtree::findIntersectionInTree(vector3d rayOrigin, vector3d rayVector, vector3d &outIntersectionPoint, node *current, bool &wasIntersection, triangle &intersectedTriangle)
{
	if (current->childs.empty())
	{
		vector3d tempIntersectionPoint;
		double distanceOld;
		double distanceNew;
		for (int i = 0; i < current->triangles.size(); i++)
		{
			if (intersectionChecker::rayIntersectsTriangle(rayOrigin, rayVector, &(current->triangles[i]), tempIntersectionPoint))
			{
				if (wasIntersection)
				{
					distanceOld = (rayOrigin - outIntersectionPoint).getLength();
					distanceNew = (rayOrigin - tempIntersectionPoint).getLength();
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
		bool wasIntersectionWithRect;
		for (int i = 0; i < current->childs.size(); i++)
		{
			wasIntersectionWithRect = intersectionChecker::intersectionRayAndBox(rayVector, rayOrigin, current->childs[i]);
			if (wasIntersectionWithRect)
			{
				findIntersectionInTree(rayOrigin, rayVector, outIntersectionPoint, current->childs[i], wasIntersection, intersectedTriangle);
			}
		}

		return wasIntersection;
	}
}

bool rtree::findIntersectionLigthInTree(vector3d lightPoint, vector3d lightVector, node *current, vector3d &intersectionPoint, triangle &intersectedTriangle)
{
	bool triangleBefore = false;
	if (current->childs.empty())
	{
		double distanceOld;
		double distanceNew;
		vector3d tempIntersectionPoint;
		for (int i = 0; i < current->triangles.size() && !triangleBefore; i++)
		{
			if (intersectionChecker::rayIntersectsTriangle(lightPoint, lightVector, &(current->triangles[i]), tempIntersectionPoint))
			{
				if (!(current->triangles[i] == intersectedTriangle))
				{
					distanceOld = (lightPoint - intersectionPoint).getLength();
					distanceNew = (lightPoint - tempIntersectionPoint).getLength();
					if (distanceNew < distanceOld)
					{
						triangleBefore = true;
					}
				}
			}
		}
		return triangleBefore;
	}
	else
	{
		for (int i = 0; i < current->childs.size() && !triangleBefore; i++)
		{
			if (intersectionChecker::intersectionRayAndBox(lightVector, lightPoint, current->childs[i]))
			{
				triangleBefore = findIntersectionLigthInTree(lightPoint, lightVector, current->childs[i], intersectionPoint, intersectedTriangle);
			}
		}
		return triangleBefore;
	}
}

double rtree::getVolume(node* current)
{
	return ((current->x_max - current->x_min) * (current->y_max - current->y_min) * (current->z_max - current->z_min));
}

node* rtree::getRoot()
{
	return root;
}
