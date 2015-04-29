#include "Converter.h"
#include <QDebug>
#include <QCoreApplication>
#include <QMutex>

#include <SDL.h>
#include <vector>
#include <iostream>

#include "Picture.h"
#include "FileWriter.h"
#include "Compressor.h"

/** Konstruktor klasy Converter
@param openPath ścieżka, pod którą znajduje się plik BMP do konwersji
@param savePath ścieżka wskazująca miejsce zapisu pliku po konwersji
@param saveName nazwa pliku po konwersji
@param colorType konwersja w kolorze lub skali szarości
 */
Converter::Converter(const string& openPath, const string& savePath, const string& saveName, const int& colorType) :
    openPath(openPath), savePath(savePath), saveName(saveName), colorType(colorType)
{
}

/** Funkcja przeprowadzająca cały proces konwersji pliku BMP do pliku DT.
*/
void Converter::run() {
    emit conversionStart();
    emit conversionProgress(5);

    QCoreApplication::processEvents();
    QMutex mutex;
    mutex.lock();

    Picture *picture = new Picture(openPath, colorType);
    if(!picture->checkBMPFile()) {
        emit conversionFailed();
        emit finished();
        mutex.unlock();
        emit conversionEnd();

        return;
    }

    FileWriter *writer = new FileWriter(savePath, saveName); //obiekt klasy służącej do zapisu obrazu do pliku

    emit conversionProgress(20);

    vector<SDL_Color> colorsList = picture->getPictureColors(); //pobranie kolorow z obrazka

    emit conversionProgress(40);

    Compressor *compressor = new Compressor(colorsList, picture);
    vector<int> pixelListAfterCompression = compressor->getPixels(); //właściwa kompresja LZW

    emit conversionProgress(90);

    writer->saveFile(picture, pixelListAfterCompression, colorsList, compressor->getMaxIndex()); //zapis do pliku


    delete picture;
    delete compressor;
    delete writer;

    mutex.unlock();
    emit conversionSuccess();
    emit conversionEnd();
}

