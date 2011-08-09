include(../../../tests.pri)

TARGET = tst_symbian_label
TEMPLATE = app
QT += declarative

SOURCES += tst_quickcomponentslabel.cpp
OTHER_FILES += \
    tst_quickcomponentslabel.qml

include(../../auto.pri)
