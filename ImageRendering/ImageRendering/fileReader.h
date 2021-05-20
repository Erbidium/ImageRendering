#pragma once
#include "triangle.h"
#include <vector>
#include <string>

class fileReader
{
public:
	static std::vector<triangle>readObj(std::string nameOfFile);
};

