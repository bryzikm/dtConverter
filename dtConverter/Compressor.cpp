#include <SDL/SDL.h>
#include <vector>
#include <algorithm>
#include <iostream>

#include "Compressor.h"

using namespace std;

/** Konstruktor klasy tworzący początkowe hasła słownika
@param startColors lista kolorów obrazka
@param picture obiekt klasy Picture bądący reprezentacją obrazka wejściowego (DI)
*/
Compressor::Compressor(const vector<SDL_Color>& startColors, Picture* picture) {
    this->picture = picture;
    this->dictionarySize = 0;
    this->maxIndex = 0;

    //przepisanie kolorów obrazka jako początkowych elementów słownika
    for(vector<SDL_Color>::const_iterator it = startColors.begin(); it != startColors.end(); it++) {
        vector<SDL_Color> color;
        color.push_back(*it);
        dictionarySize++;
        dictionary.insert(pair<int, vector<SDL_Color> >(dictionarySize , color));
    }
}

/** Własciwa kompresja LZW
@return lista kolejno zapisanych pixeli wg. indeksów slownika LZW
*/
vector<int> Compressor::getPixels() {
    vector<int> output;
    vector<SDL_Color> c;


    int x = picture->getPictureWidth(),
        y = picture->getPictureHeight();

    for(int i = 0; i < x; i++) { //dla kazdego pixel w obrazku realizujemy algorytm kompresji LZW
        for(int j = 0; j < y; j++) {
            SDL_Color color = this->getPixel(i, j);

            vector<SDL_Color> temp = c;
            temp.push_back(color);

            if(this->getDictionaryIndex(temp) > 0) { //jezeli c+s jest w slowniku
                 // c = c+s
                 c.push_back(color);
            } else {
                int index = this->getDictionaryIndex(c);

                //zapisujemy największy użyty do tej pory indeks ze słownika LZW
                if(index > this->maxIndex) {
                   this->maxIndex = index;
                }

                output.push_back(index); //dodajemy indeks c na wyjscie

                c.push_back(color); //c = c+s

                dictionarySize++; //zwiekszamy licznik hasel w slowniku
                dictionary.insert(pair<int, vector<SDL_Color> >(dictionarySize, c)); //dodanie nowego hasla do slownika

                c.clear(); //c = s
                c.push_back(color);
            }
        }

    }

    //dla ostatniego pixela
    if(!c.empty()) {
        output.push_back(this->getDictionaryIndex(c));
    }

    return output;
}

/**
@param x polozenie pixela na osi x
@param y polozenie pixela na osi y
@return obiekt klasy SDL_Color bedacy reprezentacją piksela o współrzędnych (x,y) w odpowiednim trybie koloru
*/
SDL_Color Compressor::getPixel(const int& x, const int& y) {
    return picture->getPixelColor(x,y);
}

/**
@param c lista kolorów, którą należy odszukać w słowniku
@return pozycja w słowniku lub -1 jeżeli słowa nie ma w słowniku
*/
int Compressor::getDictionaryIndex(const vector<SDL_Color>& c) {

    //dla kazdego elementu mapy dictionary sprawdzamy czy vector<DT_Color&> maja takie same wartosci/kolory (nie referencje)!!

    for(map<int, vector<SDL_Color> >::iterator it = dictionary.begin(); it != dictionary.end(); ++it) {
        if((*it).second.size() == c.size()) { //jezeli listy maja takie same rozmiary

            auto pred = [=] (const SDL_Color& first, const SDL_Color& second) {
                return ((first.r == second.r) && (first.b == second.b) && (first.g == second.g));
            };
            //sprawdzamy czy struktury zawierają takie same elementy używając pred
            if(equal((*it).second.begin(), (*it).second.end(), c.begin(), pred)) {
                return (*it).first;
            }
        }
    }

    return -1; //-1 jezeli c nie ma w slowniku
}

/**
@return największy użyty indeks ze słownika LZW
 */
int Compressor::getMaxIndex() {
    return this->maxIndex;
}

