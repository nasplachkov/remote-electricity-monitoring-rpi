#-------------------------------------------------
#
# Project created by QtCreator 2014-04-12T18:21:42
#
#-------------------------------------------------

QT += core network serialport
QT -= gui

TARGET = remote-electricity-monitoring
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    httpserver.cpp \
    pst05.cpp \
    pst05data.cpp

HEADERS += \
    httpserver.h \
    pst05.h \
    pst05query.h \
    pst05data.h

LIBS += -L$$PWD/../ -lqhttpserver

INCLUDEPATH += $$PWD/../qhttpserver
DEPENDPATH += $$PWD/../qhttpserver
