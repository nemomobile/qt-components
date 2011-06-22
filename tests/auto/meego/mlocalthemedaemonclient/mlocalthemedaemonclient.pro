include (../../../tests.pri)

INCLUDEPATH += ../../../../src/meego

DEFINES += THEME_DIR=\\\"\"$$THEME_DIR\"\\\"

TEMPLATE = app
SOURCES += \
    tst_mlocalthemedaemonclient.cpp \
    ../../../../src/meego/themedaemon/mabstractthemedaemonclient.cpp \
    ../../../../src/meego/themedaemon/mlocalthemedaemonclient.cpp

HEADERS += \
    ../../../../src/meego/themedaemon/mabstractthemedaemonclient.h \
    ../../../../src/meego/themedaemon/mlocalthemedaemonclient.h

include (../../auto.pri)
