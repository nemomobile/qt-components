include (../../../tests.pri)

TARGET = tst_multipleviews
TEMPLATE = app

SOURCES += tst_multipleviews.cpp
OTHER_FILES += tst_multipleviews.qml

CONFIG += testcase
INCLUDEPATH += ../../../../src/meego/

include (../../auto.pri)

