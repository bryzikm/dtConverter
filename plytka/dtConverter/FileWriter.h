#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <SDL/SDL.h>

#include "Picture.h"

using namespace std;

class FileWriter
{
public:
    FileWriter(const string& savePath, const string& saveName);
    void saveFile(Picture *picture, const vector<int>& pixelListAfterCompression, const vector<SDL_Color>& colorsList, const int& maxIndex);

private:
    string convertValueToBinary(int value, const int& precision);
    string savePath;
    string saveName;
};

#endif
