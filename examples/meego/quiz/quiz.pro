include(../../../qt-components.pri)
TEMPLATE = app
QT += declarative

# Input
SOURCES += main.cpp

OTHER_FILES += \
    Core/Button.qml \
    Core/Display.qml \
    mainwindow.qml \
    questionpage.qml \
    answerpage.qml

include(../meego.pri)
