#-------------------------------------------------
#
# Project created by QtCreator 2014-12-17T17:22:51
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32:RC_FILE = application.rc
TARGET = dtConverter
TEMPLATE = app
SOURCES +=\
        mainwindow.cpp \
    Compressor.cpp \
    FileWriter.cpp \
    Picture.cpp \
    Converter.cpp \
    Decoder.cpp \
    Reader.cpp \
    main.cpp

HEADERS  += mainwindow.h \
    Compressor.h \
    FileWriter.h \
    Picture.h \
    Converter.h \
    decoder.h \
    reader.h

FORMS    += mainwindow.ui

LIBS  +=  -L SDL-1.2.14/lib -lSDL
INCLUDEPATH += SDL-1.2.14/include


RESOURCES += \
    resources.qrc

OTHER_FILES += \
    application.rc





