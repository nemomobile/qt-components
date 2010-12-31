include(../../../qt-components.pri)

TEMPLATE = app
QT += declarative

CONFIG -= app_bundle

# Input
SOURCES += calculator.cpp

OTHER_FILES += \
    calculator.qml \
    page.qml \
    Core

include(../meego.pri)
