CONFIG += ordered

TEMPLATE = subdirs

SUBDIRS += qhttpserver \
    src

settings.path = $$OUT_PWD/build
settings.files = settings.ini

INSTALLS += settings
