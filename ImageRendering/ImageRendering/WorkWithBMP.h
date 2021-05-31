#pragma once
#include "PIXEL.h"
#include <vector>


class workWithBMP
{
public:
	static void createBMPImage(int depth, int width, std::vector<std::vector<PIXEL>>& image);
};
