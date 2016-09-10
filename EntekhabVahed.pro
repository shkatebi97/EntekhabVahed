QT      += core gui

TARGET = worldisnotenough #what you want to call your exe

SOURCES += main.cpp \
    units.cpp

HEADERS += headername.h \
    units.h

QT       += core gui   #for using qtcore and qtgui stuff

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets  #for qt5

