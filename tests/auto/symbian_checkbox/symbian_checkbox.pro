include (../../tests.pri)

TARGET = tst_symbian_checkbox
TEMPLATE = app
QT += declarative

SOURCES += \
    tst_checkbox.cpp

include (../auto.pri)

OTHER_FILES += \
    tst_checkbox.qml
