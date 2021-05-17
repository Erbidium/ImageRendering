#pragma once
#include "triangle.h"
#include <vector>
#include <iostream>
#include <string>
#include "vector3d.h"
#include <fstream>
using namespace std;
class fileReader
{
public:
	static vector<triangle>readObj(string nameOfFile);
};

