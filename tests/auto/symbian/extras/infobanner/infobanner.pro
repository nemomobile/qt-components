include(../../../../tests.pri)

TARGET = tst_symbian_infobanner
TEMPLATE = app
QT += declarative

SOURCES += tst_infobanner.cpp
OTHER_FILES += tst_infobanner.qml

include(../../../auto.pri)
