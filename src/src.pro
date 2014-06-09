#-------------------------------------------------
#
# Project created by QtCreator 2014-04-12T18:21:42
#
#-------------------------------------------------

QT += network serialport
QT -= gui

DESTDIR = $$OUT_PWD/../build
TARGET = remote-electricity-monitoring

TEMPLATE = app

SOURCES += main.cpp \
    httpserver.cpp \
    pst05.cpp \
    pst05data.cpp \
    pst05store.cpp

HEADERS += \
    httpserver.h \
    pst05.h \
    pst05query.h \
    pst05data.h \
    pst05store.h

LIBS += -L$$PWD/../ -lqhttpserver

INCLUDEPATH += $$PWD/../qhttpserver
DEPENDPATH += $$PWD/../qhttpserver
