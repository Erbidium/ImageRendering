#include "fileReader.h"
#include <fstream>
#include <algorithm>

using namespace std;

vector<triangle> fileReader::readObj(string nameOfFile)
{
	vector<triangle> triangles;
	vector<vector3d> tops;
	ifstream inFile(nameOfFile);
	string s;
	triangle tempTriangle;
	while (getline(inFile,s)) {
		vector3d tempTop;
		if ((s.size()>=2)&&(s[0] == 'v')&&(s[1]!='t')&&(s[1]!='n')) {
			int k = 0;
			k = s.find(' ');
			s.erase(0, k + 1);
			k = s.find(' ');
			double x = stod(s.substr(0, k));
			s.erase(0, k+1);
			k = s.find(' ');
			double y = stod(s.substr(0, k));
			s.erase(0, k + 1);
			k = s.find(' ');
			double z = stod(s.substr(0, k));
			s.erase(0, k + 1);
			tempTop.setX(x);
			tempTop.setY(y);
			tempTop.setZ(z);
			tops.push_back(tempTop);
		}
		else if (s[0] == 'f') {
			int countSlash = count(s.begin(), s.end(), '/');
			if (countSlash == 6 && s.find('//')) {
				triangle tempTriangle;
				int k = s.find(' ');
				s.erase(0, k + 1);
				k = s.find("//");
				int firstTop = stoi(s.substr(0, k));
				s.erase(0, k + 2);
				k = s.find(' ');
				s.erase(0, k + 1);
				k = s.find("//");
				int secondTop = stoi(s.substr(0, k));
				s.erase(0, k + 2);
				k = s.find(' ');
				s.erase(0, k + 1);
				k = s.find("//");
				int thirdTop = stoi(s.substr(0, k));
				s.erase(0, k + 2);
				tempTriangle.setVertex1(tops[firstTop - 1]);
				tempTriangle.setVertex2(tops[secondTop - 1]);
				tempTriangle.setVertex3(tops[thirdTop - 1]);
				triangles.push_back(tempTriangle);
			}
			else if (countSlash == 6 && !s.find('//')) {
				triangle tempTriangle;
				int k = s.find(' ');
				s.erase(0, k + 1);
				k = s.find('/');
				int firstTop = stoi(s.substr(0, k));
				s.erase(0, k + 1);
				k = s.find(' ');
				s.erase(0, k + 1);
				k = s.find("/");
				int secondTop = stoi(s.substr(0, k));
				s.erase(0, k + 1);
				k = s.find(' ');
				s.erase(0, k + 1);
				k = s.find("/");
				int thirdTop = stoi(s.substr(0, k));
				s.erase(0, k + 1);				
				tempTriangle.setVertex1(tops[firstTop - 1]);
				tempTriangle.setVertex2(tops[secondTop - 1]);
				tempTriangle.setVertex3(tops[thirdTop - 1]);
				triangles.push_back(tempTriangle);
			}
			else if (countSlash == 8) {
				triangle tempTriangle1;
				triangle tempTriangle2;
				int k = s.find(' ');
				s.erase(0, k + 1);
				k = s.find('/');
				int firstTop = stoi(s.substr(0, k));
				s.erase(0, k + 1);
				k = s.find(' ');
				s.erase(0, k + 1);
				k = s.find("/");
				int secondTop = stoi(s.substr(0, k));
				s.erase(0, k + 1);
				k = s.find(' ');
				s.erase(0, k + 1);
				k = s.find("/");
				int thirdTop = stoi(s.substr(0, k));
				s.erase(0, k + 1);
				k = s.find(' ');
				s.erase(0, k + 1);
				k = s.find("/");
				int fourthTop = stoi(s.substr(0, k));
				s.erase(0, k + 1);
				/*cout << "firstTop " << firstTop << endl;
				cout << "secondTop " << secondTop << endl;
				cout << "thirdTop " << thirdTop << endl;
				cout << "fourthTop " << fourthTop << endl;
				cout << " Check " << endl;
				tops[firstTop - 1].print();*/
				tempTriangle1.setVertex1(tops[firstTop - 1]);
				tempTriangle1.setVertex2(tops[secondTop - 1]);
				tempTriangle1.setVertex3(tops[thirdTop - 1]);
				tempTriangle2.setVertex1(tops[firstTop - 1]);
				tempTriangle2.setVertex2(tops[thirdTop - 1]);
				tempTriangle2.setVertex3(tops[fourthTop- 1]);
				triangles.push_back(tempTriangle1);
				triangles.push_back(tempTriangle2);
			}
		}
	}
	return triangles;
}

void fileReader::takeParameters(string& pathToFile, vector3d& cameraPosition, vector3d& lookAtPoint, vector<vector3d>& lightPosition, int& widthOfScreen, int& heightOfScreen, vector<int>& intens, vector<vector3d>& lightColour, vector3d& lightModel)
{
	string dataFile;
	cout<<"Enter name of file: ";
	cin>>dataFile;
	ifstream inFile(dataFile);
	double temp;
	string info;
	inFile>>info;
	inFile>>pathToFile;
	inFile>>info;
	inFile >> temp;
	cameraPosition.setX(temp);
	inFile >> temp;
	cameraPosition.setY(temp);
	inFile >> temp;
	cameraPosition.setZ(temp);
	inFile>>info;
	inFile >> temp;
	lookAtPoint.setX(temp);
	inFile >> temp;
	lookAtPoint.setY(temp);
	inFile >> temp;
	lookAtPoint.setZ(temp);
	inFile>>info;
	int numberOfLightSources;
	inFile >> numberOfLightSources;
	lightPosition.resize(numberOfLightSources);
	lightColour.resize(numberOfLightSources);
	intens.resize(numberOfLightSources);
	for (int i = 0; i < numberOfLightSources; i++)
	{
		inFile>>info;
		inFile >> temp;
		lightPosition[i].setX(temp);
		inFile >> temp;
		lightPosition[i].setY(temp);
		inFile >> temp;
		lightPosition[i].setZ(temp);
		inFile>>info;
		inFile >> intens[i];
		inFile>>info;
		inFile >> temp;
		lightColour[i].setX(temp / 255);
		inFile >> temp;
		lightColour[i].setY(temp / 255);
		inFile >> temp;
		lightColour[i].setZ(temp / 255);
	}
	inFile>>info;
	inFile >> temp;
	lightModel.setX(temp/255);
	inFile >> temp;
	lightModel.setY(temp/255);
	inFile >> temp;
	lightModel.setZ(temp/255);
	inFile>>info;
	inFile >> widthOfScreen;
	inFile>>info;
	inFile >> heightOfScreen;
	inFile.close();
}