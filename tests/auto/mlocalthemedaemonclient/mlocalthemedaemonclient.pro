include (../../tests.pri)

INCLUDEPATH += ../../../src/meego/themebridge

CONFIG += testcase

DEFINES += THEME_DIR=\\\"\"/usr/share/themes\"\\\"

TEMPLATE = app
SOURCES += \
    tst_mlocalthemedaemonclient.cpp \
    ../../../src/meego/themebridge/themedaemon/mabstractthemedaemonclient.cpp \
    ../../../src/meego/themebridge/themedaemon/mlocalthemedaemonclient.cpp

HEADERS += \
    ../../../src/meego/themebridge/themedaemon/mabstractthemedaemonclient.h \
    ../../../src/meego/themebridge/themedaemon/mlocalthemedaemonclient.h
