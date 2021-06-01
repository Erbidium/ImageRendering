#include "graphicalScene.h"

int main()
{
	graphicalScene myScene;
	myScene.buildTree();
	myScene.renderPixels();
	myScene.pixelsToFile();
}