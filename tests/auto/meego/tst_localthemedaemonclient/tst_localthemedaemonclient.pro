TARGET = tst_localthemedaemonclient
include(../test_common.pri)

DEFINES += THEME_DIR=\\\"\"$$THEME_DIR\"\\\"

SOURCES += \
    tst_localthemedaemonclient.cpp \
    ../../components/src/themedaemon/mabstractthemedaemonclient.cpp \
    ../../components/src/themedaemon/mlocalthemedaemonclient.cpp

HEADERS += \
    ../../components/src/themedaemon/mabstractthemedaemonclient.h \
    ../../components/src/themedaemon/mlocalthemedaemonclient.h
