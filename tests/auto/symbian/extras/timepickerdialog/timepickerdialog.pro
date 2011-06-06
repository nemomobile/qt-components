include(../../../../tests.pri)

TARGET = tst_symbian_timepickerdialog
TEMPLATE = app
QT += declarative

SOURCES += tst_timepickerdialog.cpp
OTHER_FILES += tst_timepickerdialog.qml

include(../../../auto.pri)
