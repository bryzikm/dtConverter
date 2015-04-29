#ifndef DECODER_H_INCLUDED
#define DECODER_H_INCLUDED

#include <iostream>
#include <string>
#include <cstddef>
#include <fstream>
#include <vector>
#include <map>
#include<iostream>
#include<SDL/SDL.h>

using namespace std;

class Decoder
{

public:
    int power(int liczba, int dopotegi);
    int bin2dec(string input);
    string charToString(char *buffer,int lenght);
    void binaryPixelToRGB(string binaryPixel);

    void drawPicture(string name_save);
    void saveBMP(string name_save);
    void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B);

    SDL_Surface *screen;
    int height;
    int width;
    char const* tytul = "BitMapPreview";
    protected:
    int maxColors;
    int pixelWidth;
    int dictionaryStart;
    int pictureStart;
    vector<SDL_Color> pixels;
    map<int,string> dictionaryColors;

Decoder()
{
    height=300;
    width=300;
}

};


#endif // DECODER_H_INCLUDED
