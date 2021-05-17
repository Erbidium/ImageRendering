#pragma once
#include "PIXEL.h"
#include <vector>


class WorkWithBMP
{
public:
	static void createBMPImage(int depth, int width, std::vector<std::vector<PIXEL>>& image);
};
