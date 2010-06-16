include (../../qt-components.pri)

TEMPLATE = lib
TARGET = mxlikeplugin
DESTDIR = Mx
CONFIG += qt plugin
QT += declarative

HEADERS += \
        mxlikewindow.h \
        mxlikebuttongroup.h \
	mxtoplevelitem.h \
	mxtoplevelitem_p.h

SOURCES += \
        mxlikewindow.cpp \
        mxlikebuttongroup.cpp \
	mxtoplevelitem.cpp \
        plugin.cpp

OTHER_FILES += \
    Mx/Button.qml \
    Mx/Window.qml \
    Mx/TopLevelItem.qml \
    Mx/TooltipLoader.qml \
    Mx/Tooltip.qml \
    Mx/Toggle.qml \
    Mx/TitlebarCombobox.qml \
    Mx/Slider.qml \
    Mx/ScrollView.qml \
    Mx/ScrollBar.qml \
    Mx/ProgressBar.qml \
    Mx/PathBar.qml \
    Mx/PathBar.js \
    Mx/MyLoader.qml \
    Mx/MyLoader.js \
    Mx/Label.qml \
    Mx/Expander.qml \
    Mx/Entry.qml \
    Mx/Combobox.qml
