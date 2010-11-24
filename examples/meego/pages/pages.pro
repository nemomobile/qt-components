include(../../../qt-components.pri)
TEMPLATE = app
QT += declarative

# Input
SOURCES += pages.cpp

OTHER_FILES += \
    main.qml \
    mainpage.qml

include(../meego.pri)
