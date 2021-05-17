#include "fileReader.h"

int main()
{
	vector<triangle> triangles =  fileReader::readObj("cow.obj");
	cout << triangles[triangles.size()-1].getVertex1().getX();

}