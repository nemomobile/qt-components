TEMPLATE = app
QT += declarative
include(../../../qt-components.pri)

# Input
SOURCES += calculator.cpp

OTHER_FILES += \
    calculator.qml \
    page.qml \
    Core/Button.qml \
    Core/calculator.js \
    Core/Display.qml
