#include <fstream>
#include <vector>
#include "WorkWithBMP.h"
#include "BMP.h"

using namespace std;

void WorkWithBMP::createBMPImage(int depth, int width, vector<vector<PIXEL>>& image)
{
    BMP header;
    ofstream out("result.bmp", ios::binary);
    char str[10] = {};
    int padding = (4 - (width) % 4) % 4;
    out.seekp(0, ios_base::beg);
    header.filesize = sizeof(BMP) + sizeof(PIXEL) * header.width * header.depth + padding * header.depth;
    header.width = width;
    header.depth = depth;
    header.headersize = sizeof(BMP);
    out.write((char*)&header, sizeof(BMP));
    for (int i = 0; i < depth; i++) {
        for (int j = 0; j < width; j++) {
            out.write((char*)&(image[i][j]), sizeof(PIXEL));
        }
        out.write(str, padding);
    }
    out.close();
}