QHTTPSERVER_BASE = .

TEMPLATE = lib

TARGET = qhttpserver

QT += network
QT -= gui

CONFIG += staticlib

DEFINES += QHTTPSERVER_EXPORT

INCLUDEPATH += $$QHTTPSERVER_BASE/http-parser

PRIVATE_HEADERS += $$QHTTPSERVER_BASE/http-parser/http_parser.h qhttpconnection.h

PUBLIC_HEADERS += qhttpserver.h qhttprequest.h qhttpresponse.h qhttpserverapi.h qhttpserverfwd.h

HEADERS = $$PRIVATE_HEADERS $$PUBLIC_HEADERS
SOURCES = *.cpp $$QHTTPSERVER_BASE/http-parser/http_parser.c

DESTDIR = ../../remote-electricity-monitoring-rpi
