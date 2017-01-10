QT += core
QT -= gui

CONFIG += c++11

TARGET = ssdll-test
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../ssdll/
LIBS += -L"$$PWD/../libs/"
LIBS += -lssdll

SOURCES += main.cpp \
    translatordemo.cpp

HEADERS += \
    translatordemo.h
