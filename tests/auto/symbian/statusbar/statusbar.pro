include(../../../tests.pri)

TARGET = tst_symbian_statusbar
TEMPLATE = app
QT += declarative

SOURCES += tst_quickcomponentsstatusbar.cpp
OTHER_FILES += \
    tst_quickcomponentsstatusbar.qml

include(../../auto.pri)
