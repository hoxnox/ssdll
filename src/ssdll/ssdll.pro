#-------------------------------------------------
#
# Project created by QtCreator 2017-01-04T20:43:22
#
#-------------------------------------------------

TARGET = ssdll
TEMPLATE = lib
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += SSDLL_LIBRARY

SOURCES += lookupdictionary.cpp \
    dictionarymetadata.cpp \
    dictziplib.cpp \
    dictionarybase.cpp \
    utils.cpp \
    worddata.cpp \
    indexfile.cpp

HEADERS += lookupdictionary.h\
        ssdll_global.h \
    dictionarymetadata.h \
    dictziplib.hpp \
    mapfile.hpp \
    dictionarybase.h \
    utils.h \
    worddata.h \
    indexfile.h

INCLUDEPATH += ../

unix {
    LIBS += -lz
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}
