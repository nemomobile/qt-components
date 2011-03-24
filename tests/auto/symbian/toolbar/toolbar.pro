include(../../../tests.pri)

TARGET = tst_symbian_toolbar
TEMPLATE = app
QT += declarative

SOURCES += tst_quickcomponentstoolbar.cpp
OTHER_FILES += tst_quickcomponentstoolbar.qml
RESOURCES += toolbar.qrc

include(../../auto.pri)
