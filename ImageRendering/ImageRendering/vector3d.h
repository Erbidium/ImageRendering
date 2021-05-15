#pragma once

class vector3d
{
	double x;
	double y;
	double z;
public:
	vector3d():x(0),y(0),z(0){}
	vector3d(double newX, double newY, double newZ):x(newX),y(newY),z(newZ){}
	double getX()const{return x;}
	double getY()const{return y;}
	double getZ()const{return z;}
	void setX(double newX){x=newX;}
	void setY(double newY){y=newY;}
	void setZ(double newZ){z=newZ;}
};
