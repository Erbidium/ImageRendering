#include "intersectionChecker.h"
#include <cmath>

using namespace std;

bool intersectionChecker::rayIntersectsTriangle(vector3d rayOrigin, vector3d rayVector, triangle* inTriangle, vector3d& outIntersectionPoint)
{
    const double EPSILON=0.0000001;
    vector3d vertex1=inTriangle->getVertex1();
    vector3d vertex2=inTriangle->getVertex2();  
    vector3d vertex3=inTriangle->getVertex3();
    vector3d edge1=vertex2-vertex1;
    vector3d edge2=vertex3-vertex1;
    vector3d h=rayVector.crossProduct(edge2);
    double a=edge1*h;
    if((a>-EPSILON)&&(a<EPSILON))
    {
	    return false;
    }
    double f=1.0/a;
    vector3d s=rayOrigin-vertex1;
    double u=f*(s*h);
    if(u<0.0||u>1.0)
    {
    	return false;
    }
    vector3d q=s.crossProduct(edge1);
    double v=f*(rayVector*q);
    if(v<0.0||u+v>1.0)
    {
	    return false;
    }
    double t=f*(edge2*q);
    if(t>EPSILON)
    {
        outIntersectionPoint=rayOrigin+rayVector*t;
        return true;
    }
    else
        return false;
}

bool intersectionChecker::intersectionRayAndBox(vector3d direction, vector3d origin, Node* current)
{
    vector3d maxValues(current->x_max, current->y_max, current->z_max);
    vector3d minValues(current->x_min, current->y_min, current->z_min);
    float txmin = (minValues.getX() - origin.getX()) / direction.getX();
    float txmax = (maxValues.getX() - origin.getX()) / direction.getX();

    if (txmin > txmax) swap(txmin, txmax);
    
    float tymin = (minValues.getY() - origin.getY()) / direction.getY();
    float tymax = (maxValues.getY() - origin.getY()) / direction.getY();

    if (tymin > tymax) swap(tymin, tymax);

    if ((txmin > tymax) || (tymin > txmax))
        return false;

    if (tymin > txmin)
        txmin = tymin;

    if (tymax < txmax)
        txmax = tymax;

    float tzmin = (minValues.getZ() - origin.getZ()) / direction.getZ();
    float tzmax = (maxValues.getZ() - origin.getZ()) / direction.getZ();

    if (tzmin > tzmax) swap(tzmin, tzmax);

    if ((txmin > tzmax) || (tzmin > txmax))
        return false;

    return true;
}
