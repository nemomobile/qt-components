include(../../../qt-components.pri)
TEMPLATE = app
QT += declarative

# Input
SOURCES += gallery.cpp

OTHER_FILES += \
    main.qml \
    buttongroups.qml \
    button.qml \
    checkbox.qml \
    lineedit.qml \
    progressbar.qml \
    switches.qml \
    slider.qml \
    list.qml \
    scrolldecorator.qml \
    query.qml \
    busyindicator.qml \
    images

include (../meego.pri)
