#include "intersectionChecker.h"


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


bool intersectionChecker::intersectionRayAndBox(vector3d ray, vector3d rayOrigin, Node*current)
{
	vector3d maxVertex(current->x_max, current->y_max, current->z_max);
	vector3d minVertex(current->x_min, current->y_min, current->z_min);
	double tmin, tmax, tymin, tymax, tzmin, tzmax;
	int sign[3];
	vector3d invdir={1/ray.getX(), 1/ray.getY(), 1/ray.getZ()};
	vector3d bounds[2];
	bounds[0] = minVertex; 
    bounds[1] = maxVertex; 
	sign[0] = (invdir.getX() < 0); 
    sign[1] = (invdir.getY() < 0); 
    sign[2] = (invdir.getZ() < 0); 
    tmin = (bounds[sign[0]].getX() - rayOrigin.getX()) * invdir.getX(); 
    tmax = (bounds[1-sign[0]].getY() - rayOrigin.getX()) * invdir.getX(); 
    tymin = (bounds[sign[1]].getY() - rayOrigin.getY()) * invdir.getY(); 
    tymax = (bounds[1-sign[1]].getY() - rayOrigin.getY()) * invdir.getY(); 
    if((tmin>tymax)||(tymin>tmax)) 
        return false; 
    if(tymin>tmin) 
        tmin=tymin; 
    if (tymax<tmax) 
        tmax=tymax; 
    tzmin=(bounds[sign[2]].getZ() - rayOrigin.getZ()) * invdir.getZ(); 
    tzmax=(bounds[1-sign[2]].getZ() - rayOrigin.getZ()) * invdir.getZ(); 
    return ((tmin>tzmax)||(tzmin>tmax))?false:true;
	/*
	https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
	*/
}