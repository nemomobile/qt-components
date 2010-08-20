TEMPLATE = app
QT += declarative
include(../../../qt-components.pri)

# Input
SOURCES += calculator.cpp

OTHER_FILES += \
    Core/Button.qml \
    Core/Display.qml \
    mainwindow.qml \
    questionpage.qml \
    answerpage.qml
