CONFIG += ordered

TEMPLATE = subdirs

SUBDIRS += qhttpserver \
    src

install_settings.path = $$OUT_PWD/src
install_settings.files = settings.ini

INSTALLS += install_settings
