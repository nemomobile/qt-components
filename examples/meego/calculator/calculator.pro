include(../../../qt-components.pri)
TEMPLATE = app
QT += declarative

# Input
SOURCES += calculator.cpp

OTHER_FILES += \
    calculator.qml \
    page.qml \
    Core/Button.qml \
    Core/calculator.js \
    Core/Display.qml
