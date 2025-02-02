#-------------------------------------------------
#
# Project created by QtCreator 2017-01-04T20:43:22
#
#-------------------------------------------------

TARGET = ssdll
TEMPLATE = lib
CONFIG += c++14
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += SSDLL_LIBRARY
DEFINES += "_FILE_OFFSET_BITS=64"

!win32 {
    QMAKE_CXXFLAGS += -std=c++14
}

SOURCES += lookupdictionary.cpp \
    dictionarymetadata.cpp \
    dictziplib.cpp \
    utils.cpp \
    worddata.cpp \
    indexfile.cpp \
    mapfile.cpp \
    basicdictionary.cpp \
    dictionaryprivate.cpp \
    logger.cpp

HEADERS += lookupdictionary.h\
        ssdll_global.h \
    dictionarymetadata.h \
    dictziplib.hpp \
    mapfile.hpp \
    utils.h \
    worddata.h \
    indexfile.h \
    basicdictionary.h \
    dictionaryprivate.h \
    logger.h

INCLUDEPATH += ../

win32 {
    DEFINES += ZLIB_WINAPI \
               ZLIB_DLL
    LIBS += -L"$$PWD/../libs/"

    # xpiks build
    CONFIG(debug, debug|release) {
        LIBS += -L"$$PWD/../../../../libs/debug"
    } else {
        LIBS += -L"$$PWD/../../../../libs/release"
    }

    INCLUDEPATH += ../zlib-1.2.11/
    INCLUDEPATH += ../../../zlib-1.2.11/
    LIBS += -lz
    LIBS += -lmman
    LIBS += -lWs2_32
}

appveyor {
    LIBS += -L"$$PWD/../../../../libs/"
}

unix {
    LIBS += -lz
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

linux-g++-64 {
    LIBS += -lz
    CONFIG += staticlib
    target.path = "$$PWD/../../../libs/"

}
