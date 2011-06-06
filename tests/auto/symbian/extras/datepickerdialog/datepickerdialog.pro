include(../../../../tests.pri)

TARGET = tst_symbian_datepickerdialog
TEMPLATE = app
QT += declarative

SOURCES += tst_datepickerdialog.cpp
OTHER_FILES += tst_datepickerdialog.qml

include(../../../auto.pri)
