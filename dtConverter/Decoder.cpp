#include"decoder.h"
#include "reader.h"
#include "FileWriter.h"

/** Funkcja ustawiajaca pixel
@param x współrzędna x obrazka
@param y współrzędna x obrazka
@param R składowa R koloru ustawianego piksela
@param G składowa G koloru ustawianego piksela
@param B składowa B koloru ustawianego piksela
 */
void Decoder::setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B)
{
    if ((x>=0) && (x<width) && (y>=0) && (y<height))
    {
        /* Zamieniamy poszczególne sk3adowe koloru na format koloru pixela */
        Uint32 pixel = SDL_MapRGB(screen->format, R, G, B);

        /* Pobieramy informacji ile bajtów zajmuje jeden pixel */
        int bpp = screen->format->BytesPerPixel;

        /* Obliczamy adres pixela */
        Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;

        /* Ustawiamy wartooa pixela, w zale?nooci od formatu powierzchni*/
        switch(bpp)
        {
        case 1: //8-bit
            *p = pixel;
            break;

        case 2: //16-bit
            *(Uint16 *)p = pixel;
            break;

        case 3: //24-bit
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4: //32-bit
            *(Uint32 *)p = pixel;
            break;

        }
        /* update the screen (aka double buffering) */
    }
}

/** Funkcja zamiany liczby zapisanej w systemie szesnastkowym na system dziesiętny
@param hex liczba zapisana w systemie szesnastkowym
*/

int Decoder::hex2dec(string hex)
{
    int dec = strtol(hex.c_str(), NULL, 16);
    return dec;
}

/** Funkjca rzutujaca typ char na string
@param buffer bufor danych
@param length długość bufora
 */
string Decoder::charToString(char *buffer,int lenght)
{
    string ciag="";
    for(int i=0; i<lenght; i++)
    {
        ciag = ciag+buffer[i];
    }
    return ciag;
}



/** Funkcja odpowiedzialna za zapis do pliku
@param name_save nazwa pliku do którego zapisujemy obraz
 */
void Decoder::saveBMP(string name_save)
{

    size_t found = name_save.find(".bmp");

    if(found==-1)
    {
        name_save = name_save + ".bmp";
    }

    char *nazwa_save= new char[name_save.length()+1];
    strcpy( nazwa_save, name_save.c_str() );

    SDL_SaveBMP (screen, nazwa_save);

}

/** Funkcja zmieniajaca odczyt binarny piksela na skladowe RGB
@param binaryPixel binarny zapis piksela
*/
void Decoder::binaryPixelToRGB(string binaryPixel){
    int j = 0;
    int binaryPixelSize = binaryPixel.size();
    string binaryPixelsArray[binaryPixelSize];
    SDL_Color kolor[binaryPixelSize];

    for(int i=0; i<binaryPixelSize/6 ; i++){
        binaryPixelsArray[i] = binaryPixel.substr(j,6);
        j = j+6;
    }

    for(int i=0; i<binaryPixelSize/6; i++){
        int red = hex2dec(binaryPixelsArray[i].substr(0,2));
        int green = hex2dec(binaryPixelsArray[i].substr(2,2));
        int blue = hex2dec(binaryPixelsArray[i].substr(4,2));
        kolor[i].r = red;
        kolor[i].g = green;
        kolor[i].b = blue;
        pixels.push_back(kolor[i]);
    }
}

/** Funkcja tworzaca obraz
@param name_save nazwa pliku wynikowego
 */
void Decoder::drawPicture(string name_save)
{
    SDL_Color bitMapPixel;

    int pixelIndex =-1;
    for(int x=0; x<width; x++)
    {
        SDL_Flip(screen);
        for(int y=0; y<height; y++)
            {

                bitMapPixel = pixels[pixelIndex];
                setPixel(x,y,bitMapPixel.r,bitMapPixel.g,bitMapPixel.b);
                pixelIndex++;
            }
    }



    bitMapPixel = pixels[0];
    setPixel(0,0,bitMapPixel.r,bitMapPixel.g,bitMapPixel.b);

         saveBMP(name_save);
}





