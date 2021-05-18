#include "fileReader.h"

vector<triangle> fileReader::readObj(string nameOfFile)
{
	vector<triangle> triangles;
	vector<vector3d> tops;
	ifstream inFile(nameOfFile);
	string s;
	triangle tempTriangle;
	while (getline(inFile,s)) {
		vector3d tempTop;
		if (s[0] == 'v') {
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
			tempTriangle.setVertex1(tops[firstTop-1]);
			tempTriangle.setVertex2(tops[secondTop-1]);
			tempTriangle.setVertex3(tops[thirdTop-1]);
			//cout<<firstTop<<" "<<secondTop<<" "<<thirdTop<<endl;
			triangles.push_back(tempTriangle);
		}
	}
	return triangles;
}
